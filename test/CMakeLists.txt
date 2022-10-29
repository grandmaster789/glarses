cmake_minimum_required(VERSION 3.15)

find_package(Catch2 3 REQUIRED)
include(Catch)

add_executable(
  unittest

  "unittest.h"
  "unittest.cpp"
  "io/test_file.cpp" 
  "util/test_string.cpp"
)

target_include_directories(unittest PUBLIC ../glarses)

target_link_libraries(
  unittest PRIVATE
  glarses_lib
  Catch2::Catch2WithMain
)

catch_discover_tests(unittest)

# this figures out all of the required dll's and copies everything to the output folder
add_custom_command(TARGET unittest PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:unittest> $<TARGET_FILE_DIR:unittest>
  COMMAND_EXPAND_LISTS
)