#pragma once

#include <iosfwd>
#include <cstdint>

#include "../../dependencies.h"

namespace glarses::opengl::buffers {
	// NOTE the values are picked so they correspond to GLenum values, allowing this to be cast directly
	//
	enum class e_DataType :
		uint16_t
	{
		i8  = 0x1400, // (signed) byte/char
		u8  = 0x1401, // unsigned byte/char
		i16 = 0x1402, // (signed) short
		u16 = 0x1403, // unsigned short
		i32 = 0x1404, // (signed) int
		u32 = 0x1405, // unsigned int
		f32 = 0x1406, // float
		f64 = 0x140A  // double
	};

	std::ostream& operator << (std::ostream& os, e_DataType t);

	GLenum to_GLenum(e_DataType t);
}