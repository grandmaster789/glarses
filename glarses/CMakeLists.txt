add_library(glarses_lib 
	"glarses.cpp"
)

find_package(glfw3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# Manually add the Tilt5 NDK to the project
target_include_directories(glarses_lib PUBLIC "${PROJECT_SOURCE_DIR}/dependencies/include")

add_library(TiltFiveNative SHARED IMPORTED)
set_property(TARGET TiltFiveNative PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/dependencies/lib/win64/TiltFiveNative.dll")
set_property(TARGET TiltFiveNative PROPERTY IMPORTED_IMPLIB "${PROJECT_SOURCE_DIR}/dependencies/lib/win64/TiltFiveNative.dll.if.lib")

target_link_libraries(glarses_lib PUBLIC 
	glfw
	fmt::fmt
	TiltFiveNative
)