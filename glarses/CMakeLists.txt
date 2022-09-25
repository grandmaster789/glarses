add_library(glarses_lib 
	"glarses.cpp"
)

find_package(glfw3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

target_link_libraries(glarses_lib PRIVATE 
	glfw
	fmt::fmt
)