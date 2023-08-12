# Glarses
Experimental simulation engine with the TiltFive AR glasses NDK

This is mostly for experimentation, but the project itself is pretty advanced - the goal is to make a custom game engine that is suitable for high performance simulation, targeting the tiltfive hardware as the primary display and interaction mechanism.

As a sidenote - most of the code has very minimal dependencies, but the structure is very much suited to simple extension with other libraries. 

I'm interested in figuring out how things work with a fair amount of detail, so that's what this project is also reflecting.

References:
- Vulkan Lecture Series https://www.youtube.com/watch?v=tLwbj9qys18&list=PLmIqTlJ6KsE1Jx5HV4sd2jOe3V1KMHHgn
- A Gentle introduction to Vulkan for Rendering and Compute Workloads https://www.youtube.com/watch?v=nD83r06b5NE
- Using Vulkan Hpp, The C++ Header for Vulkan https://www.youtube.com/watch?v=KFsbrhcqW2U 
- https://www.youtube.com/user/cmyuksel
- Mastering Graphics Programming with Vulkan https://www.packtpub.com/product/mastering-graphics-programming-with-vulkan/9781803244792
- vcpkg Manifest Mode https://learn.microsoft.com/en-us/vcpkg/users/manifests

Current tiltfive features available:
- reasonably robust background thread that detects connections/disconnections
- vulkan-based graphics
- permission escalation to exclusive access
- 

WIP
- Mirrored rendering to a window of either eyeglass
- Wand input
- Custom AR user interface elements
- (windows) Crash dump reporting
- (windows) Stack tracing

TODO
- Memory model should be updated to PMR style
  - maybe mimalloc would be nice: // https://github.com/microsoft/mimalloc
- Plugin architecture with hot reloading
- coroutine-based task scheduling
- background I/O operations
- Simple ECS
- https://learn.microsoft.com/en-us/windows/win32/gdi/capturing-an-image (see if this is GDI only)
- this seems promising: https://gitlab.com/ribtoks/chillout

Game engine features:
- Reasonably multithreaded handling of external devices
- Explicit memory management
- Robust internal communication channels between different threads

Current project-level features:
- CMakeList based project
- Mixed external dependencies: vcpkg packages, direct source includes, binary libraries
- C++20 code
- unit testing via Catch2
