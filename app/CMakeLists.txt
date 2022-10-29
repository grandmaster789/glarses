add_executable(glarses_app "main.cpp")

target_include_directories(glarses_app PRIVATE "../glarses")

target_link_libraries(glarses_app PRIVATE 
	glarses_lib
)

# this figures out all of the required dll's and copies everything to the output folder
# (from https://cmake.org/cmake/help/git-stage/manual/cmake-generator-expressions.7.html#genex:TARGET_RUNTIME_DLLS)
add_custom_command(TARGET glarses_app PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:glarses_app> $<TARGET_FILE_DIR:glarses_app>
  COMMAND_EXPAND_LISTS
)

