add_library(glarses_lib 
	"glarses.cpp"
	"window.h" 
	"window.cpp"
	"dependencies.h" 
	"dependencies.cpp"  
	"cube.h"	
	"cube.cpp" 
	
	

	"io/file.h" 
	"io/file.cpp" 

	"math/transform.h"
	"math/transform.cpp"
	"math/rect.h" 

	"t5/t5_manager.h" 
	"t5/t5_manager.cpp"  
	"t5/t5_glasses.h" 
	"t5/t5_glasses.cpp"

	"opengl/render_target.h" 
	"opengl/render_buffer.h" 
	"opengl/render_buffer.cpp" 
	"opengl/frame_buffer.h" 
	"opengl/frame_buffer.cpp" 
	"opengl/render_target.cpp"
	"opengl/shader_program.cpp" 
	"opengl/texture.h" 
	"opengl/texture.cpp" 
	"opengl/uniform_buffer.h" 
	"opengl/uniform_buffer.cpp" 
	"opengl/shader_program.h"

	"util/algorithm.h"
	"util/flat_map.h" 
	"util/string.h" 
	"util/string.cpp" 	
	"util/mediator.h" 
	"util/detail/mediator_queue.h"
 "player.h" "player.cpp" "application.h" "application.cpp")

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
