# based on https://github.com/aminya/cpp_vcpkg_project
# notable changes - this uses a local copy of vcpkg instead of system-wide
#                   a lot of references to additional tooling was removed; I don't have those installed and I'm not spending a weekend getting it to work

cmake_minimum_required(VERSION 3.21)

# set a default CXX standard for the tools and targets that do not specify them.
# If commented, the latest supported standard for your compiler is automatically set.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add project_options v0.22.4
# https://github.com/aminya/project_options
# Change the version in the following URL to update the package (watch the releases of the repository for future updates)
include(FetchContent)
FetchContent_Declare(_project_options URL https://github.com/aminya/project_options/archive/refs/tags/v0.22.4.zip)
FetchContent_MakeAvailable(_project_options)
include(${_project_options_SOURCE_DIR}/Index.cmake)

# project features
option(FEATURE_TESTS      "Enable the tests" ON)
if (FEATURE_TESTS)
    list(APPEND VCPKG_MANIFEST_FEATURES "tests")
endif()

# package manager
#run_vcpkg()                 # install system-wide vcpkg dependencies: - should be called before defining project()
set(VCPKG_VERSION latest)    # use 'edge' for bleeding edge, 'latest' for latest release, or a release tag for a specific (pinned) version
include(cmake/vcpkg.cmake)   # download + bootstrap + dependency management

# Set the project name and language
project(glarses
        VERSION 0.0.1
        DESCRIPTION ""
        HOMEPAGE_URL "https://github.com/grandmaster789/glarses"
        LANGUAGES CXX C)

# Initialize project_options variable related to this project
# This overwrites `project_options` and sets `project_warnings`
# uncomment to enable the options. Some of them accept one or more inputs:
project_options(
        ENABLE_CACHE
        ${ENABLE_CPPCHECK}
        ${ENABLE_CLANG_TIDY}
        # ENABLE_INTERPROCEDURAL_OPTIMIZATION
        # ENABLE_NATIVE_OPTIMIZATION
        ${ENABLE_DOXYGEN}
        ${ENABLE_COVERAGE}
        ${ENABLE_SANITIZER_ADDRESS}
        # ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
        # ENABLE_SANITIZER_LEAK
        # ENABLE_SANITIZER_THREAD
        # ENABLE_SANITIZER_MEMORY
        # ENABLE_PCH
        # PCH_HEADERS
        WARNINGS_AS_ERRORS
        # ENABLE_INCLUDE_WHAT_YOU_USE
        # ENABLE_USER_LINKER
        # ENABLE_BUILD_WITH_TIME_TRACE
        # ENABLE_UNITY
)

add_subdirectory(./app)
add_subdirectory(./glarses)

if(FEATURE_TESTS)
    enable_testing()
    add_subdirectory(./test)
endif()

# the variables set using CACHE STRING "" are passed to package_project
# Package the project
package_project(
        TARGETS
        glarses_app
        glarses_lib
        project_options
        project_warnings
        INTERFACE_DEPENDENCIES_CONFIGURED
        INTERFACE_INCLUDES
        PUBLIC_INCLUDES
        ${GLARSES_INCLUDE_DIR})

# set the startup project for the "play" button in MSVC
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT app)

# Experience shows that explicit package naming can help make it easier to sort
# out potential ABI related issues before they start, while helping you
# track a build to a specific GIT SHA
set(CPACK_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
    )

include(CPack)