#include "application.h"
#include "t5/t5_glasses.h"

namespace {
	std::string intepret_glfw_errorcode(int code) {
		switch (code) {
		case GLFW_NOT_INITIALIZED:		return "GLFW has not been initialized";
		case GLFW_NO_CURRENT_CONTEXT:	return "No context is current for this thread";
		case GLFW_INVALID_ENUM:			return "One of the arguments to the function was an invalid enum value";
		case GLFW_INVALID_VALUE:		return "One of the arguments to the function was an invalid value";
		case GLFW_OUT_OF_MEMORY:		return "A memory allocation failed";
		case GLFW_API_UNAVAILABLE:		return "GLFW could not find support for the requested client API on the system";
		case GLFW_VERSION_UNAVAILABLE:	return "The requested OpenGL or OpenGL ES version is not available";
		case GLFW_PLATFORM_ERROR:		return "A platform - specific error occurred that does not match any of the more specific categories";
		case GLFW_FORMAT_UNAVAILABLE:	return "The requested format is not supported or available";
		default:
			return std::string("Unknown error code: ") + std::to_string(code);
		}
	}

	void glfw_error_callback(int code, const char* description) {
		std::cerr << "GLFW error (" << intepret_glfw_errorcode(code) << "): " << description << '\n';
	}
}

namespace glarses {
	Application::Application() {
		glfwSetErrorCallback(&glfw_error_callback);

		if (!glfwInit())
			throw std::runtime_error("Failed to initialize GLFW");

		// set up an openGL 4.6 window with the Core profile
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE); // enable sRGB
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	}

	Application::~Application() {
		glfwTerminate();
	}

	void Application::run() {
		// wait until we have at least one player
		// (players will be started once a pair of t5 glasses is found)
		// 
		// starting a player creates a window for that player
		//
		std::cout << "Waiting for players to join\n";

		while (true) {
			if (!m_FoundGlasses.empty()) {
				init_found_glasses();
				break;
			}

			using namespace std::chrono_literals;
			std::this_thread::sleep_for(100ms);
		}

		// now update all players until none are left
		// (players exit by closing their window, pressing escape or disconnecting their glasses)

		while (!m_Players.empty()) {
			auto current_playerset = util::weak_copy(m_Players); // create a local copy to allow players to remove themselves from the set

			for (auto& player : current_playerset)
				player->update();

			glfwPollEvents(); // process all pending OS events (keypresses, window events, etc.)
		}
	}

	// this will be called from another thread; access should be synchronized
	void Application::operator()(const t5::Manager::GlassesFound& found) {
		std::lock_guard guard(m_FoundGlassesMutex);
		m_FoundGlasses.push_back(found.m_Glasses);
	}

	void Application::operator()(const Player::Quit& player_quitting) {
		std::erase_if(
			m_Players, [&](const std::unique_ptr<Player>& p) { 
				return p.get() == player_quitting.m_Player;
			}
		);
	}

	void Application::init_found_glasses() {
		// transfer to this thread
		std::vector<t5::Glasses*> found;

		{
			std::lock_guard guard(m_FoundGlassesMutex);

			if (!m_FoundGlasses.empty())
				std::swap(m_FoundGlasses, found);
			else
				return;
		}

		// create a new Player object for each pair of glasses
		// (includes a window and wand listener)
		for (auto* obj : found)
			m_Players.push_back(std::make_unique<Player>(make_player_name(), obj));
	}

	std::string Application::make_player_name() {
		static int s_PlayerID = 1;
		std::stringstream sstr;
		sstr << "Player " << s_PlayerID++;
		return sstr.str();
	}
}