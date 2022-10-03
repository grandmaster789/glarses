add_library(glarses_lib 
	"glarses.cpp"
	"window.h" 
	"window.cpp"
 "dependencies.h" "dependencies.cpp"  "cube.h" "shader_program.h" "cube.cpp" "io/file.h" "io/file.cpp" "util/string.h" "util/string.cpp" "shader_program.cpp" "texture.h" "texture.cpp" "uniform_buffer.h" "uniform_buffer.cpp" "render_target.h"   "render_target.cpp")

find_package(fmt CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

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
)