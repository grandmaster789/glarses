# Glarses Project assorted notes
Glarses is a personal research project for interacting with a Tilt5 headset. A x64 windows native SDK was provided, so that's nice.
Initial impressions is that at this point more effort was put into the Unity plugin; understandable, but still slightly sad.

I'm hoping to implement the computer vision algorithms myself, and figure out a way to turn this into a low-cost CAVE setup.

# Project dependencies
Obviously this requires a Tilt5 headset; although part of the development will be geared towards making a usable debugging environment as well.

Project setup was done in Visual Studio 2022 Community; it's CMake based, and should also work in CLion or VSCode but I haven't put any effort into making that work.

The project uses VCPKG as a package manager in manifest mode, which should auto-install when running CMake. It's a local version, so it should not interfere with other projects on the system.

# Unity SDK analysis
The plugin seems to be compressed in a .tar.gz type blob, simple enough to extract everything.
Maybe it would be nice to include a tool to decompress everything and rename it to their internal asset filenames.
The Csharp code inside should be a good guide to getting some initial results, as it ostensibly makes use of the NDK.

# SimpleViewer
Interestingly this shows that there are several different cameras on board; looks like both are some kind of IR camera; the wand has several IR leds in the antenna part which show up super bright in the viewer. 
The streams shown in the viewer are not currently exposed in the NDK, after asking on Discord it seems that this is a planned feature for a future release. I might still be able to figure it out from the viewer program, but there's enough other stuff to setup anyway.