add_library(glarses_lib
			"glarses.cpp"
			"window.cpp"
			"dependencies.cpp"
			"application.cpp"

			"io/file.cpp"
			"io/bitmap.cpp"

			"math/extent2d.cpp"
			"math/extent3d.cpp"

			"t5/t5_manager.cpp"
			"t5/t5_glasses.cpp"

			"util/string.cpp"
			"util/filesystem.cpp"
			math/offset2d.cpp math/offset3d.cpp opengl/gltypes.cpp)

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
