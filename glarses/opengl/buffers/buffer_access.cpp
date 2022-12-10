#include "buffer_access.h"

#include <ostream>

namespace glarses::opengl::buffers {
	GLenum to_GLenum(e_BufferAccess ba) {
		return static_cast<GLenum>(ba);
	}

	std::ostream& operator << (std::ostream& os, e_BufferAccess ba) {
		switch (ba) {
		case e_BufferAccess::stream_draw: os << "stream_draw"; break;
		case e_BufferAccess::stream_read: os << "stream_draw"; break;
		case e_BufferAccess::stream_copy: os << "stream_draw"; break;

		case e_BufferAccess::dynamic_draw: os << "dynamic_draw"; break;
		case e_BufferAccess::dynamic_read: os << "dynamic_read"; break;
		case e_BufferAccess::dynamic_copy: os << "dynamic_copy"; break; 
		
		case e_BufferAccess::static_draw: os << "static_draw"; break;
		case e_BufferAccess::static_read: os << "static_read"; break;
		case e_BufferAccess::static_copy: os << "static_copy"; break;

		default:
			os << "[UNKNOWN]";
		}

		return os;
	}
}