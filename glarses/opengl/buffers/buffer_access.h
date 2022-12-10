#pragma once

#include <iosfwd>
#include <cstdint>

#include "../../dependencies.h"

namespace glarses::opengl::buffers {
	// NOTE the values are picked so they correspond to GLenum values, allowing this to be cast directly
	//
	enum class e_BufferAccess:
		uint16_t
	{
		stream_draw  = 0x88E0,
		stream_read  = 0x88E1,
		stream_copy  = 0x88E2,
		dynamic_draw = 0x88E8,
		dynamic_read = 0x88E9,
		dynamic_copy = 0x88EA,
		static_draw  = 0x88E4,
		static_read  = 0x88E5,
		static_copy  = 0x88E6
	};

	GLenum to_GLenum(e_BufferAccess ba);

	std::ostream& operator << (std::ostream& os, e_BufferAccess ba);
}