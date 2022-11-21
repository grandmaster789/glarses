# Glarses
Experimental simulation engine with the TiltFive AR glasses NDK

This is mostly for experimentation, but the project itself is pretty advanced - the goal is to make a custom game engine that is suitable for high performance simulation, targeting the tiltfive hardware as the primary display and interaction mechanism.

As a sidenote - most of the code has very minimal dependencies, but the structure is very much suited to simple extension with other libraries. 

I'm interested in figuring out how things work with a fair amount of detail, so that's what this project is also reflecting.

Current tiltfive features available:
- reasonably robust background thread that detects connections/disconnections
- minimal openGL initialization code
- permission escalation to exclusive access
- 

WIP
- Mirrored rendering to a window of either eyeglass
- Wand input
- Custom AR user interface elements

Game engine features:
- Reasonably multithreaded handling of external devices
- Robust internal communication channels between different threads
- TODO Simple ECS
- TODO coroutine-based task scheduling
- TODO background I/O operations

Current project-level features:
- CMakeList based project
- Vcpkg package management
- C++20 code
- unit testing via Catch2
