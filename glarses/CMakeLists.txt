add_library(glarses_lib
			"glarses.cpp"
			"dependencies.cpp"
			"application.cpp"

			"io/file.cpp"
			"io/bitmap.cpp"

			"log/log_category.cpp"
			"log/log_message.cpp"
			"log/log_sink.cpp"
			"log/logger.cpp"

			"os/window.cpp"

			"util/atomic.cpp"
			"util/active_object.cpp"
			"util/string.cpp"
			"util/filesystem.cpp"
			"util/detail/memory_statistics.cpp"
			"util/detail/heap_allocator.cpp"
			"util/detail/stack_allocator.cpp"
			"util/detail/double_stack_allocator.cpp"
			"util/detail/linear_allocator.cpp"
			"util/detail/mallocator.cpp"
			"util/memory.cpp"
			"os/crash_handler_windows.cpp"
)

# package-based dependencies (vcpkg)
find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Boost REQUIRED)
find_package(Stb REQUIRED)

find_path(VULKAN_INCLUDE vulkan/vulkan.h PATH "$ENV{VULKAN_SDK}/include")
find_path(RANG_INCLUDE_DIRS "rang.hpp")
find_path(PARALLEL_HASHMAP_INCLUDE_DIRS "parallel_hashmap/btree.h")

# Manually add the Tilt5 NDK to the project, as well as header-only library paths
target_include_directories(glarses_lib PUBLIC 
	"${PROJECT_SOURCE_DIR}/dependencies/include"
	${Stb_INCLUDE_DIR}
	${VULKAN_INCLUDE}
	${RANG_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
	${PARALLEL_HASHMAP_INCLUDE_DIRS}
)

add_library(TiltFiveNative SHARED IMPORTED)

if (WIN32)
	set_property(TARGET TiltFiveNative PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/dependencies/lib/win/x86_64/TiltFiveNative.dll")
	set_property(TARGET TiltFiveNative PROPERTY IMPORTED_IMPLIB "${PROJECT_SOURCE_DIR}/dependencies/lib/win/x86_64/TiltFiveNative.dll.if.lib")

	add_definitions(-DVK_USE_PLATFORM_WIN32_KHR) # use Windows WSI

	option(ENABLE_VALIDATION "Enable vulkan validation" ON)

	if (ENABLE_VALIDATION)
		add_definitions(-DENABLE_VALIDATION)
	endif()
endif()

target_link_libraries(glarses_lib PUBLIC 
	glfw
	fmt::fmt
	glm::glm
	TiltFiveNative
	glarses_dependencies
)
