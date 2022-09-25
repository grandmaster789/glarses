add_executable(glarses_app "main.cpp")

target_include_directories(glarses_app PRIVATE "../glarses")

target_link_libraries(glarses_app PRIVATE 
	glarses_lib
)