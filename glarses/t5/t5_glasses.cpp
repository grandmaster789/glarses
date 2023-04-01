#include "t5_glasses.h"
#include "t5_manager.h"

namespace glarses::t5 {
	Glasses::Glasses(const std::string& hardware_id) {
		auto err = t5CreateGlasses(
            Manager::instance().get_context(),
            hardware_id.data(),
            &m_Handle
        );

        if (err)
            throw std::runtime_error(t5GetResultMessage(err));
	}

	Glasses::~Glasses() {
        cleanup();
	}

	Glasses::Glasses(Glasses&& g) noexcept :
        m_Handle (g.m_Handle)
	{
        g.m_Handle = nullptr;
	}

	Glasses& Glasses::operator= (Glasses&& g) noexcept {
        cleanup();

        m_Handle   = g.m_Handle;
        g.m_Handle = nullptr;

		return *this;
	}

    bool Glasses::init(
        std::string_view display_name,
        GLFWwindow*      window_handle
    ) {
        {
            auto err = t5ReserveGlasses(m_Handle, display_name.data());
            if (err) {
                std::cout << "Failed to reserve glasses: " << t5GetResultMessage(err) << '\n';
                return false;
            }
        }

        std::cout << query_connection_state() << '\n'; // should be 'exclusive reservation'

        // try to ensure ready state
        // (observed anywhere between 1 and 100 retries with 100ms delays upon first connecting before this succeeds)
        static constexpr int k_MaxRetries = 25;
        static int total_num_attempts = 0;

        for (int i = 0; i < k_MaxRetries; ++i) {
            ++total_num_attempts;
            auto err = t5EnsureGlassesReady(m_Handle);

            if (err == T5_SUCCESS)
                break;

            if (i == k_MaxRetries - 1)
                return false;

            if (err == T5_ERROR_TRY_AGAIN) {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(100ms);
                std::cout << ".";
                continue;
            }

            throw std::runtime_error(t5GetResultMessage(err));
        }

        std::cout << "Total number of ensure attempts: " << total_num_attempts << '\n';
        std::cout << query_connection_state() << '\n'; // should be 'exclusive connection'

        m_IPD = query_ipd();
        std::cout << "IPD: " << (m_IPD * 1000.0) << "mm\n";

        // initialize graphics context
        // NOTE it seems there are some undocumented settings that can be passed here
        {
            glfwMakeContextCurrent(window_handle);

            auto err = t5InitGlassesGraphicsContext(
                m_Handle,
                T5_GraphicsApi::kT5_GraphicsApi_GL,
                nullptr                             // graphics context
            );

            if (err)
                throw std::runtime_error(t5GetResultMessage(err));
        }

        return true;
    }

    std::optional<glm::vec3> Glasses::get_position() const {
        // should this be cached? the mutex situation also doesn't quite seem right

        std::lock_guard guard(m_Mutex);

        return glm::vec3(
            m_Pose.posGLS_GBD.x,
            m_Pose.posGLS_GBD.y,
            m_Pose.posGLS_GBD.z
        );
    }

    std::optional<glm::quat> Glasses::get_orientation() const {
        // should this be cached? the mutex situation also doesn't quite seem right

        std::lock_guard guard(m_Mutex);

        return glm::quat(
            m_Pose.rotToGLS_GBD.x,
            m_Pose.rotToGLS_GBD.y,
            m_Pose.rotToGLS_GBD.z,
            m_Pose.rotToGLS_GBD.w
        );
    }

    std::optional<T5_GameboardType> Glasses::get_board_type() const {
        // should this be cached? the mutex situation also doesn't quite seem right

        std::lock_guard guard(m_Mutex);

        return m_Pose.gameboardType;
    }

    void Glasses::poll() {
        {
            T5_GlassesPose pose;
            auto err = t5GetGlassesPose(
                m_Handle,
                kT5_GlassesPoseUsage_GlassesPresentation,
                &pose
            );

            if (err == T5_SUCCESS) {
                std::lock_guard guard(m_Mutex);
                m_Pose = pose;
            }
        }
    }

    void Glasses::cleanup() noexcept {
        if (m_Handle) {
            // I've tried releasing the glasses, but that doesn't seem to work? Not sure why though

            t5DestroyGlasses(&m_Handle);
        }
    }

    T5_ConnectionState Glasses::query_connection_state() const {
        T5_ConnectionState result;

        auto err = t5GetGlassesConnectionState(m_Handle, &result);

        if (err != T5_SUCCESS)
            throw std::runtime_error(t5GetResultMessage(err));

        return result;
    }

    double Glasses::query_ipd() const {
        double result;

        auto err = t5GetGlassesFloatParam(m_Handle, 0, kT5_ParamGlasses_Float_IPD, &result);
        if (err != T5_SUCCESS)
            throw std::runtime_error(t5GetResultMessage(err));

        return result;
    }

    std::ostream& operator << (std::ostream& os, T5_ConnectionState state) {
        switch (state) {
            case T5_ConnectionState::kT5_ConnectionState_Disconnected:             os << "Disconnected";               break;
            case T5_ConnectionState::kT5_ConnectionState_ExclusiveConnection:      os << "Exclusive connection";       break;
            case T5_ConnectionState::kT5_ConnectionState_ExclusiveReservation:     os << "Exclusive reservation";      break;
            case T5_ConnectionState::kT5_ConnectionState_NotExclusivelyConnected:  os << "Not exclusively connected";  break;
            default:
                os << "[UNKNOWN]";
        }

        return os;
    }
}