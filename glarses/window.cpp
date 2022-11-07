#include "window.h"
#include "dependencies.h"
#include "cube.h"
#include "io/file.h"

#include "t5/t5_client.h"
#include "t5/t5_gameboard.h"
#include "t5/t5_glasses.h"

#include <stdexcept>
#include <iostream>
#include <array>
#include <string>

namespace {
	bool g_HaveWindow = false;
	
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

	void GLAPIENTRY opengl_debug_callback(
		GLenum,          // source
		GLenum,          // type
		GLuint,          // id
		GLenum,          // severity
		GLsizei,         // message size
		GLchar* message,    
		const void*      // userParam
	) {
		std::cerr << "OpenGL debug message: " << message << '\n';
	}

	std::filesystem::path find_asset_folder() {
		using namespace std::filesystem;

		auto pwd = current_path();

		while (!pwd.empty()) {
			if (exists(pwd / "assets"))
				return pwd / "assets";

			pwd = pwd.parent_path();
		}

		throw std::runtime_error("Failed to find the asset folder");
	}
}

namespace glarses {
	Window::Window() {
		if (g_HaveWindow)
			throw std::runtime_error("Only one window is allowed...");
		else
			g_HaveWindow = true;

		glfwSetErrorCallback(&glfw_error_callback);

		if (!glfwInit())
			throw std::runtime_error("Failed to initialize GLFW");

		// set up an openGL 4.6 window with the Core profile
		glfwWindowHint(GLFW_CLIENT_API,            GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SRGB_CAPABLE,          GL_TRUE); // enable sRGB

		m_Handle = glfwCreateWindow(
			800,		// width
			600,        // height
			"Glarses",  // title
			nullptr,    // monitor
			nullptr     // shared opengl context
		);

		if (m_Handle == nullptr)
			throw std::runtime_error("Failed to create a window");

		glfwMakeContextCurrent(m_Handle);
		glfwSwapInterval(1); // wait for 1 screen update before swapping front/back buffers (vsync)

		glfwSetInputMode(m_Handle, GLFW_STICKY_KEYS, GL_TRUE); // buffer the keypresses

		glewExperimental = true; // this is actually required for glew to try and get openGL core functions
		auto glew_err = glewInit();
		if (glew_err != GLEW_OK) {
			std::cerr << glewGetErrorString(glew_err) << '\n';
			throw std::runtime_error("Failed to initialize GLEW");
		}

		// after extensions are enabled we can enable debug callbacks
#ifdef _DEBUG
		glDebugMessageCallbackARB(reinterpret_cast<GLDEBUGPROCARB>(opengl_debug_callback), nullptr);
		if (glGetError() != GL_NO_ERROR)
			std::cerr << "glDebugMessageCallbackARB failure\n";

		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		
		// disable the 'notification' severity
		glDebugMessageControlARB(
			GL_DONT_CARE,	        		// source
			GL_DONT_CARE,					// type
			GL_DEBUG_SEVERITY_NOTIFICATION,	// severity
			0,								// count
			nullptr,						// GLuint *ids
			GL_FALSE						// enabled
		);
#endif
		auto assets = find_asset_folder();

		m_ShaderProgram = load_shader_sources(
			assets / "shaders" / "basic_triangle.vert",
			assets / "shaders" / "basic_triangle.frag"
		);

		m_PerFrameBuffer = UniformBuffer::create<glm::mat4>(0);

		m_Texture = Texture::load_file(assets / "textures" / "debug_color_02.png");
		//m_Texture.bind(0);

		// start looking for those glasses
	}

	Window::~Window() {
		glfwTerminate();
	}

	void Window::run() {
		glClearColor(0.2f, 0.0f, 0.4f, 0.0f); // purple
		glEnable(GL_DEPTH_TEST);
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);

		bool done = false;

		// create a very temporary vertex array object without any data -- the required data is embedded in the shaders
		GLuint vao = 0; 
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		auto& client = t5::Client::create("com.grandmaster.glarses", "0.0.1");
		std::cout << client << '\n';
		std::cout << client.attempt_get_service_version() << '\n';
		std::cout << "T5 attention required: " << std::boolalpha << client.attempt_is_attention_required() << '\n';

		auto changed = client.get_changed_system_parameters();
		if (changed.empty()) {
			std::cout << "No changed system parameters\n";
		}

		auto glass_ids = client.attempt_list_glasses();
		std::vector<t5::Glasses> glasses;

		for (const auto& x : glass_ids) {
			static int s_player_index = 1;
			std::stringstream sstr;
			sstr << "Player " << s_player_index++;

			glasses.push_back(client.create_glasses(x, sstr.str()));
		}

		std::cout << "XE: " << client.get_gameboard_size(T5_GameboardType::kT5_GameboardType_XE) << '\n';


		auto& player1 = glasses[0];
		player1.init_graphics();

		glfwMakeContextCurrent(m_Handle);

		while (!done) {
			T5_GlassesPose pose;
			if (player1.try_get_pose(&pose)) {
				std::cout << pose.timestampNanos << '\n';
			}

			int frame_width = 0;
			int frame_height = 0;

			glfwGetFramebufferSize(m_Handle, &frame_width, &frame_height);
			float ratio = static_cast<float>(frame_width) / std::max(frame_height, 1);

			auto time_since_app_launch = static_cast<float>(glfwGetTime());

			glViewport(0, 0, frame_width, frame_height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// rotate the triangle around the Z axis
			glm::mat4 model_matrix = glm::rotate(
				glm::mat4(1.0f),
				time_since_app_launch,
				glm::vec3(0.0f, 0.0f, 1.0f)
			);

			// orthographic (flat) projection
			glm::mat4 projection_matrix = glm::ortho(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f);

			// we don't need a view (camera) matrix yet
			auto mvp = projection_matrix * model_matrix;

			m_ShaderProgram.use();

			m_PerFrameBuffer.set_data(glm::value_ptr(mvp));

			glDrawArrays(GL_TRIANGLES, 0, 3);

			glfwSwapBuffers(m_Handle);
			glfwPollEvents();

			done |= (glfwWindowShouldClose(m_Handle) == GL_TRUE);          // someone closed the window
			done |= (glfwGetKey(m_Handle, GLFW_KEY_ESCAPE) == GLFW_PRESS); // someone pressed escape
		}

		glDeleteVertexArrays(1, &vao);
	}
}