add_library(glarses_lib
			"glarses.cpp"
			"window.cpp"
			"dependencies.cpp"
			"player.cpp"
			"application.cpp"

			"io/file.cpp"
			"io/bitmap.cpp"

			"math/transform.cpp"

			"t5/t5_manager.cpp"
			"t5/t5_glasses.cpp"

			"opengl/texture.cpp"
			"opengl/framebuffer.cpp"
			"opengl/renderbuffer.cpp"

			"util/string.cpp"
			"opengl/shader.cpp"
			"opengl/shader_program.cpp"
			"util/filesystem.cpp"
			scene/cube.cpp scene/cube.h opengl/vertex_buffer.h opengl/index_buffer.h opengl/vertex_array.h opengl/vertex_buffer.cpp opengl/vertex_array.cpp opengl/vertex.h opengl/vertex_attribute.h opengl/vertex.cpp scene/simple_shader_program.cpp scene/simple_shader_program.h)

find_package(fmt CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(gli CONFIG REQUIRED)

find_path(STB_INCLUDE_DIRS "stb.h")

# Manually add the Tilt5 NDK to the project
target_include_directories(glarses_lib PUBLIC 
	"${PROJECT_SOURCE_DIR}/dependencies/include"
	${STB_INCLUDE_DIRS}
)

add_library(TiltFiveNative SHARED IMPORTED)

if (WIN32)
	set_property(TARGET TiltFiveNative PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/dependencies/lib/win/x86_64/TiltFiveNative.dll")
	set_property(TARGET TiltFiveNative PROPERTY IMPORTED_IMPLIB "${PROJECT_SOURCE_DIR}/dependencies/lib/win/x86_64/TiltFiveNative.dll.if.lib")
endif()

target_link_libraries(glarses_lib PUBLIC 
	glfw
	fmt::fmt
	TiltFiveNative
	${OPENGL_LIBRARIES}
	GLEW::glew
	glm::glm
	gli
)
