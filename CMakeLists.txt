cmake_minimum_required(VERSION 3.21)

# set a default CXX standard for the tools and targets that do not specify them.
# If commented, the latest supported standard for your compiler is automatically set.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND VCPKG_MANIFEST_FEATURES "tests")

# package manager
#run_vcpkg()                 # install system-wide vcpkg dependencies: - should be called before defining project()
set(VCPKG_VERSION latest)    # use 'edge' for bleeding edge, 'latest' for latest release, or a release tag for a specific (pinned) version
include(cmake/vcpkg.cmake)   # download + bootstrap + dependency management

# Set the project name and language
project(glarses
        VERSION 0.0.3
        DESCRIPTION "Custom simulation engine targeting Tiltfive hardware"
        HOMEPAGE_URL "https://github.com/grandmaster789/glarses"
        LANGUAGES CXX C)

# these flags are only supported by the MSVC compiler - at some point we'll add linux/android variants
if (WIN32)
	add_definitions(-DUNICODE -D_UNICODE)      # try to use unicode support
	add_definitions(-DWIN32_LEAN_AND_MEAN)     # disable many includes from <windows.h>
	add_definitions(-D_CRT_SECURE_NO_WARNINGS) # allow printf and friends (at least the secure versions)
	add_definitions(-D_USE_MATH_DEFINES)       # mostly to define the M_PI macro
	
	add_compile_options("/MP")          # build with multiple processes
	add_compile_options("/W4")          # high warning level warnings
	add_compile_options("/permissive-") # strict adherence to the c++ standard
	add_compile_options("/utf-8")       # set source and executable character sets to UTF-8

	add_compile_options("/wd4324")      # disable warning C4324 structure was padded due to alignment specifier

	add_compile_options("/we4018")      # enable warning C4018 signed/unsigned token mismatch
	add_compile_options("/we4101")      # enable warning C4101 unreferenced local variable
	add_compile_options("/we4244")      # enable warning C4244 'argument' : conversion from 'type1' to 'type2', possible loss of data
	add_compile_options("/we4267")      # enable warning C4267 unexpected tokens following preprocessor directive
	add_compile_options("/we4305")      # enable warning C4305 'context' : truncation from 'type1' to 'type2'
	add_compile_options("/we4307")      # enable warning C4307 'operator' : integral constant overflow
	add_compile_options("/we4800")      # enable warning C4800 Implicit conversion from 'type' to bool. Possible information loss
endif()

add_subdirectory(./app)
add_subdirectory(./glarses)

enable_testing()
add_subdirectory(./test)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT app) # set the startup project for the "play" button in MSVC

# Experience shows that explicit package naming can help make it easier to sort
# out potential ABI related issues before they start, while helping you
# track a build to a specific GIT SHA
set(CPACK_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
)

include(CPack)
