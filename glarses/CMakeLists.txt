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
)

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
set_property(TARGET TiltFiveNative PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/dependencies/lib/win64/TiltFiveNative.dll")
set_property(TARGET TiltFiveNative PROPERTY IMPORTED_IMPLIB "${PROJECT_SOURCE_DIR}/dependencies/lib/win64/TiltFiveNative.dll.if.lib")

target_link_libraries(glarses_lib PUBLIC 
	glfw
	fmt::fmt
	TiltFiveNative
	${OPENGL_LIBRARIES}
	GLEW::glew
	glm::glm
	gli
)
