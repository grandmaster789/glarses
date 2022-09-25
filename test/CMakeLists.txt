cmake_minimum_required(VERSION 3.15)

find_package(Catch2 3 REQUIRED)
include(Catch)

add_executable(
  unittest

  "unittest.h"
  "unittest.cpp"
)

include_directories(../glarses)

target_link_libraries(
  unittest PRIVATE
  glarses_lib
  Catch2::Catch2WithMain
)

catch_discover_tests(unittest)