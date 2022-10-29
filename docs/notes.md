# Glarses Project assorted notes
Glarses is a personal research project for interacting with a Tilt5 headset. A x64 windows native SDK was provided, so that's nice.
Initial impressions is that at this point more effort was put into the Unity plugin; understandable, but still slightly sad.

I'm hoping to implement the computer vision algorithms myself, and figure out a way to turn this into a low-cost CAVE setup.

The current version of the NDK is 1.2.0-beta

# Project dependencies
Obviously this requires a Tilt5 headset; although part of the development will be geared towards making a usable debugging environment as well.

Project setup was done in Visual Studio 2022 Community; it's CMake based, and should also work in CLion or VSCode but I haven't put any effort into making that work.

The project uses VCPKG as a package manager in manifest mode, which should auto-install when running CMake. It's a local version, so it should not interfere with other projects on the system.

We have a choice between openGL and directX interfacing - I'm more familiar with openGL so I'm going with that. 

The current version of openGL is 4.6, although not too many people make use of that (probably because of apple and/or webGL). Using this version pretty much requires making use of an extension loader.

# Unity SDK analysis
The plugin seems to be compressed in a .tar.gz type blob, simple enough to extract everything.
Maybe it would be nice to include a tool to decompress everything and rename it to their internal asset filenames.
The Csharp code inside should be a good guide to getting some initial results, as it ostensibly makes use of the NDK.

# SimpleViewer
Interestingly this shows that there are several different cameras on board; looks like both are some kind of IR camera; the wand has several IR leds in the antenna part which show up super bright in the viewer. 
The streams shown in the viewer are not currently exposed in the NDK, after asking on Discord it seems that this is a planned feature for a future release. I might still be able to figure it out from the viewer program, but there's enough other stuff to setup anyway.

After a cursory look at which libraries are used by SimpleViewer, it seems to me that this program doesn't use the NDK at all but raw USB commands.

I asked about direct access to the video streams, and it seems that it'll be something that may become possible in the future but not right now. The cited reason is that they want to prevent unauthorized access - presumably it's a privacy thing. This is, after all, a device where it's not immediately obvious that cameras are integrated.

# NDK 
Seems like it's been designed as a C style library (probably for better compatibility). There are two 'sample' projects included that indicate intended usage. The samples don't seem to actually render anything to the glasses, I'll have to figure that out from the unity SDK I guess.

The NDK currently exposes 27 functions, the documentation adequately explains each of them. Most are of a connection/context style. 

Eventually ever frame should end up calling t5SendFrameToGlasses; both left and right images are submitted at the same time. The NDK supports either openGL or DirectX, but no Vulkan.

The T5_FrameInfo struct suggests to me that we should render into two textures, and then submit both.

The C++ sample code seems to be very much inspired by Rust, weirdly combined with shared_ptr. I consider it fairly good code, could be better of course but not bad. It's not so bad that I should write my own versions of the same operations, I'll just add some notes and it should be ok enough to use.

Because the code was written in a very Rust-like style, the generated html documentation is not quite ideal. There also seems to be quite a bit of GCC standard library documentation in there for some reason.

# Gameboard
All positions and orientations are relative to a gameboard, which is selected in the Tilt5 service. I'm fairly certain that at this point this part of the experience cannot be customized. Once I have a decent demo working, I'll have to ask in Discord what's up with that.

# Object tracking
At this point, there is no support for tracking objects other than the glasses themselves and the wands. The camera that is supposed to be used for this purpose does not have any bindings and the NDK exposes nothing on this subject.