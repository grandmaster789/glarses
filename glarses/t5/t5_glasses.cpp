#include "t5_glasses.h"
#include "t5_manager.h"

namespace glarses::t5 {
	Glasses::Glasses(std::string_view hardware_id) {
		if (auto err = t5CreateGlasses(Manager::instance().get_context(), hardware_id.data(), &m_Handle)) {
			std::cerr << "t5CreateGlasses: " << t5GetResultMessage(err) << '\n';
			m_Handle = nullptr;
		}
		else
			std::cout << "Found: " << hardware_id << '\n';
	}

	Glasses::~Glasses() {
		if (m_Handle) {
			if (m_Acquired)
				release();

			t5DestroyGlasses(&m_Handle);
		}
	}

	Glasses::Glasses(Glasses&& g) noexcept :
		m_Handle  (g.m_Handle),
		m_LastPose(g.m_LastPose),
		m_IPD     (g.m_IPD),
		m_Ready   (g.m_Ready),
		m_Acquired(g.m_Acquired)
	{
		g.m_Handle = nullptr; // for ownership purposes this is the only relevant one
	}

	Glasses& Glasses::operator= (Glasses&& g) noexcept {
		if (m_Handle) {
			if (m_Acquired)
				release();

			t5DestroyGlasses(&m_Handle);
		}

		m_Handle   = g.m_Handle;
		m_LastPose = g.m_LastPose;
		m_IPD      = g.m_IPD;
		m_Ready    = g.m_Ready;
		m_Acquired = g.m_Acquired;		

		g.m_Handle = nullptr; // for ownership purposes this is the only relevant one

		return *this;
	}

	bool Glasses::init(
		std::string_view display_name, 
		GLFWwindow*      context
	) {
        (void) display_name;

		if (!ensure_ready(20))
			return false;

		if (!initGLContext(context))
			return false;

		update_ipd();

		m_Ready = true;
		std::cout << "Glasses ready\n";

		return true;
	}

	void Glasses::poll() {
		if (m_Ready) {
			update_connection_state();
			update_pose();
		}
	}

	bool Glasses::is_pose_fresh() const {
		return (Clock::now() - m_PoseAcquiredTimestamp) < k_PoseExpiration;
	}

	const T5_GlassesPose& Glasses::get_pose() const {
		return m_LastPose;
	}

	void Glasses::release() {
		if (m_Acquired) {
			m_Acquired = false;

			if (auto err = t5ReleaseGlasses(m_Handle))
				std::cerr << "t5ReleaseGlasses: " << t5GetResultMessage(err) << '\n';
		}
	}

	bool Glasses::ensure_ready(int max_retries) {
		for (int i = 0; i < max_retries; ++i) {
			if (auto err = t5EnsureGlassesReady(m_Handle)) {
				if (err == T5_ERROR_TRY_AGAIN) {
					std::this_thread::sleep_for(k_RetryTiming);
					continue;
				}
				else {
					std::cerr << "t5EnsureGlassesReady: " << t5GetResultMessage(err) << '\n';
					return false;
				}
			}
			else
				return true;
		}

		std::cout << "Glasses::ensure_ready() timeout\n";
		return false;
	}

	bool Glasses::initGLContext(GLFWwindow* context) {
		// [NOTE] this must be called from the graphics thread
		// [NOTE] for some reason, the first call typically results in a tiltfive internal error;
		//        doing it a second time seems to fix it...
		if (auto err = t5InitGlassesGraphicsContext(m_Handle, kT5_GraphicsApi_GL, nullptr)) {
			glfwMakeContextCurrent(context);

			err = t5InitGlassesGraphicsContext(m_Handle, kT5_GraphicsApi_GL, nullptr);

			if (err) {
				std::cerr << "t5InitGlassesGraphicsContext: " << t5GetResultMessage(err) << '\n';
				return false;
			}
			else
				return true;
		}
		else
			return true;
	}

	void Glasses::update_pose() {
		T5_GlassesPose pose = {};

        // [NOTE] the usage parameter may also indicate a spectator; should redesign a bit around that
        //
		if (auto err = t5GetGlassesPose(m_Handle, kT5_GlassesPoseUsage_GlassesPresentation, &pose)) {
			// when the board isn't visible, skip notification
			if (err == T5_ERROR_TRY_AGAIN)
				return;

			std::cerr << "t5GetGlassesPose: " << t5GetResultMessage(err) << '\n';
		}
		else {
			m_PoseAcquiredTimestamp = Clock::now();
			m_LastPose = pose;
		}
	}

	void Glasses::update_connection_state() {
		T5_ConnectionState state;

		if (auto err = t5GetGlassesConnectionState(m_Handle, &state))
			std::cerr << "t5GetGlassesConnectionState: " << t5GetResultMessage(err) << '\n';
		else
			m_State = state;
	}

	void Glasses::update_ipd() {
		double ipd = 0.0;

		if (auto err = t5GetGlassesFloatParam(m_Handle, 0, T5_ParamGlasses::kT5_ParamGlasses_Float_IPD, &ipd))
			std::cerr << "t5GetGlassesFloatParam: " << t5GetResultMessage(err) << '\n';
		else
			m_IPD = ipd;
	}
}