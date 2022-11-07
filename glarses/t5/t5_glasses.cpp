#include "t5_glasses.h"

#include <ostream>
#include <iostream>

namespace t5 {
	Glasses::Glasses(
		T5_Context       ctx, 
		std::string_view id,
		std::string_view display_name
	) {
		if (auto err = t5CreateGlasses(ctx, id.data(), &m_Handle)) {
			std::cerr << "t5CreateGlasses: " << t5CreateGlasses << '\n';
			return; // maybe throw? the object is unusable after all
		}

		update_connection_state();

		set_name(display_name);
	}

	Glasses::~Glasses() {
		if (m_Handle) {
			release();
			t5DestroyGlasses(&m_Handle);
		}
	}

	Glasses::Glasses(Glasses&& g) noexcept:
		m_Handle     (g.m_Handle),
		m_DisplayName(std::move(g.m_DisplayName)),
		m_State      (g.m_State)
	{
		g.m_Handle = nullptr;
	}

	Glasses& Glasses::operator = (Glasses&& g) noexcept {
		if (m_Handle) {
			release();
			t5DestroyGlasses(&m_Handle);
		}

		m_Handle      = g.m_Handle;
		m_DisplayName = std::move(g.m_DisplayName);
		m_State       = g.m_State;

		g.m_Handle = nullptr;

		return *this;
	}

	void Glasses::set_name(std::string_view sv) {
		ExclusiveHelper helper(this);

		m_DisplayName = sv;
		
		auto err = t5SetGlassesDisplayName(m_Handle, m_DisplayName.c_str());
		if (err)
			std::cerr << "t5SetGlassesDisplayName: " << t5GetResultMessage(err) << '\n';
	}

	const std::string& Glasses::get_name() const {
		return m_DisplayName;
	}

	std::string Glasses::get_identifier() const {
		std::string buffer;
		buffer.resize(64);
		size_t sz = 64;

		// limited number of retries
		for (int i = 0; i < 4; ++i) {
			if (auto err = t5GetGlassesIdentifier(m_Handle, buffer.data(), &sz)) {
				if (err == T5_ERROR_STRING_OVERFLOW) {
					buffer.resize(sz);
					continue;
				}
				else {
					std::cerr << "t5GetGlassesIdentifier: " << t5GetResultMessage(err) << '\n';
					return {};
				}
			}
			else
				return buffer;

		}

		std::cerr << "Unknown error with t5GetGlassesIdentifier\n";
		return {};
	}

	void Glasses::init_graphics() const {
		ExclusiveHelper helper(this);

		if (auto err = t5InitGlassesGraphicsContext(
			m_Handle, 
			T5_GraphicsApi::kT5_GraphicsApi_Gl, 
			nullptr
		))
			std::cerr << "t5InitGlassesGraphicsContext: " << t5GetResultMessage(err) << '\n';
	}

	bool Glasses::try_get_pose(T5_GlassesPose* pose) const {
		ExclusiveHelper helper(this);

		if (auto err = t5GetGlassesPose(m_Handle, pose)) {
			std::cerr << "t5GetGlassesPose: " << t5GetResultMessage(err) << '\n';
			return false;
		}

		return true;
	}

	T5_ConnectionState Glasses::get_connection_state() const {
		return m_State;
	}

	T5_ConnectionState Glasses::update_connection_state() {
		auto err = t5GetGlassesConnectionState(m_Handle, &m_State);
		if (err)
			std::cerr << "t5GetGlassesConnectionState: " << t5GetResultMessage(err) << '\n';

		return m_State;
	}

	void Glasses::ensure_ready() const {
		for (int i = 0; i < k_MaxNumRetries; ++i) {
			auto err = t5EnsureGlassesReady(m_Handle);

			if (!err)
				return; // success

			if (err == T5_ERROR_TRY_AGAIN) {
				std::this_thread::sleep_for(k_RetryDelay);
				continue;
			}
			else {
				std::cerr << "t5EnsureGlassesReady: " << t5GetResultMessage(err) << '\n';
				return;
			}
			
		}

		std::cout << "ensure_ready() timeout\n";
	}

	void Glasses::acquire() const {
		auto err = t5AcquireGlasses(m_Handle, m_DisplayName.c_str());

		if (err)
			std::cerr << "t5AcquireGlasses: " << t5GetResultMessage(err) << '\n';
	}

	void Glasses::release() const {
		auto err = t5ReleaseGlasses(m_Handle);

		if (err)
			std::cerr << "t5ReleaseGlasses: " << t5GetResultMessage(err) << '\n';
	}

	std::ostream& operator << (std::ostream& os, const Glasses& g) {
		auto state = g.get_connection_state();

		os << g.get_name() << " ["; 

		switch (state) {
		case kT5_ConnectionState_Disconnected:            os << "disconnected";  break;
		case kT5_ConnectionState_ExclusiveConnection:     os << "connected";     break;
		case kT5_ConnectionState_ExclusiveReservation:    os << "reserved";      break;
		case kT5_ConnectionState_NotExclusivelyConnected: os << "non-exclusive"; break;
		default:
			os << "UNKNOWN";
		}

		os << ']';

		return os;
	}

	Glasses::ExclusiveHelper::ExclusiveHelper(const Glasses* g):
		m_Glasses(g)
	{
		g->acquire();
		g->ensure_ready();
	}

	Glasses::ExclusiveHelper::~ExclusiveHelper() {
		m_Glasses->release();
	}
}