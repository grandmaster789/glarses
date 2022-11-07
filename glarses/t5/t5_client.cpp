#include "t5_client.h"
#include "t5_glasses.h"
#include "t5_gameboard.h"

#include <ostream>

namespace t5 {
	Client::~Client() {
		t5DestroyContext(&m_Context);
	}

	Client& Client::create(
		std::string_view application_id,
		std::string_view application_version
	) {
		instance().init(application_id, application_version);
		return instance();
	}

	Client& Client::instance() {
		static Client clt;
		return clt;
	}

	T5_Context Client::get_handle() const {
		return m_Context;
	}

	std::string Client::attempt_get_service_version() const {
		std::unique_ptr<char[]> tmp(new char[T5_MAX_STRING_PARAM_LEN]);
		size_t                  sz = T5_MAX_STRING_PARAM_LEN;

		for (int i = 0; i < k_MaxRetries; ++i) {
			if (auto err = t5GetSystemUtf8Param(
				m_Context,
				T5_ParamSys::kT5_ParamSys_UTF8_Service_Version,
				tmp.get(),
				&sz
			)) {
				// if the service is unavailable, wait a bit and try again
				if (err == T5_ERROR_NO_SERVICE) {
					std::this_thread::sleep_for(k_RetryDelay);
					continue;
				}

				std::cerr << "t5GetSystemUtf8Param: " << t5GetResultMessage(err) << '\n';
				return {};
			}
			else
				return std::string(tmp.get(), sz);
		}

		std::cerr << "Client::attempt_get_service_version() timeout\n";
		return {};
	}

	bool Client::attempt_is_attention_required() const {
		int64_t x = 0;

		for (int i = 0; i < k_MaxRetries; ++i) {
			if (auto err = t5GetSystemIntegerParam(
				m_Context,
				T5_ParamSys::kT5_ParamSys_Integer_CPL_AttRequired,
				&x
			)) {
				if (err == T5_ERROR_NO_SERVICE) {
					std::this_thread::sleep_for(k_RetryDelay);
					continue;
				}

				std::cerr << "t5GetSystemIntegerParam: " << t5GetResultMessage(err) << '\n';
				return false;
			}
			else
				return !!x;
		}

		std::cerr << "Client::attempt_is_attention_required() timeout\n";
		return false;
	}

	std::vector<std::string> Client::attempt_list_glasses() const {
		// glasses have ID strings like 'XXXX-XXXX-XXXX'
		std::vector<char> buffer(64);

		size_t sz;

		int i = 0;
		for (; i < k_MaxRetries; ++i) {
			sz = buffer.size();

			T5_Result err = t5ListGlasses(m_Context, buffer.data(), &sz);

			if (!err)
				break; // T5_SUCCESS was returned, we're done
			else if (err == T5_ERROR_OVERFLOW)
				buffer.resize(sz); // increase the buffer and try again
			else if (err == T5_ERROR_NO_SERVICE) {
				// wait a bit and try again
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(10ms);
				continue;
			}
			else {
				std::cerr << "t5ListGlasses: " << t5GetResultMessage(err) << '\n';
				return {};
			}
		}

		if (i >= k_MaxRetries) {
			std::cerr << "Client::list_glasses() timeout\n";
			return {};
		}

		// buffer should be filled with null-separated strings, 
		// with an extra null terminator at the end of the list
		std::vector<std::string> result_list;

		auto ptr = buffer.data();

		while (true) {
			std::string id = ptr; // implicit conversion from c-style string (copies up to a null terminator)
			if (id.empty())
				break; // if the pointer is already at a null terminator, we've reached the end
			 
			result_list.push_back(id);

			ptr += id.length() + 1; // advance the pointer beyond the last null terminator
		}

		return result_list;
	}

	Glasses Client::create_glasses(
		std::string_view id, 
		std::string_view display_name
	) const {
		return Glasses(m_Context, id, display_name);
	}

	std::vector<T5_ParamSys> Client::get_changed_system_parameters() const {
		uint16_t num_results = 32;
		std::vector<T5_ParamSys> result(num_results);

		if (auto err = t5GetChangedSystemParams(m_Context, result.data(), &num_results)) {
			if (err == T5_ERROR_OVERFLOW) {
				result.resize(num_results);

				err = t5GetChangedSystemParams(m_Context, result.data(), &num_results);

				if (err) {
					std::cerr << "t5GetChangedSystemParams: " << t5GetResultMessage(err) << '\n';
					return {};
				}

				return result;
			}
			else
				std::cerr << "t5GetChangedSystemParams: " << t5GetResultMessage(err) << '\n';
		}

		return result;
	}

	GameboardSize Client::get_gameboard_size(T5_GameboardType type) const {
		T5_GameboardSize sz;

		auto err = t5GetGameboardSize(m_Context, type, &sz);

		if (err) {
			std::cerr << "t5GetGameboardSize: " << t5GetResultMessage(err) << '\n';
			return {};
		}
		else
			return GameboardSize(sz);
	}

	void Client::init(
		std::string_view id,
		std::string_view ver
	) {
		if (id.size() > T5_MAX_STRING_PARAM_LEN)
			throw std::runtime_error("T5 Application ID too long");

		if (ver.size() > T5_MAX_STRING_PARAM_LEN)
			throw std::runtime_error("T5 Application version too long");

		m_ClientInfo.applicationId = id.data();
		m_ClientInfo.applicationVersion = ver.data();

		if (auto result = t5CreateContext(&m_Context, &m_ClientInfo, nullptr)) {
			std::cerr << t5GetResultMessage(result) << '\n';
			throw std::runtime_error("Failed to create TiltFive Context");
		}
	}

	std::ostream& operator << (std::ostream& os, const Client& clt) {
		os << "[T5 client: " << clt.m_ClientInfo.applicationId << ']';
		return os;
	}
}