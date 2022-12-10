#include "data_type.h"

namespace glarses::opengl::buffers {
	std::ostream& operator << (std::ostream& os, e_DataType t) {
		switch (t) {
		case e_DataType::i8:  os << "byte";           break;
		case e_DataType::u8:  os << "unsigned byte";  break;
		case e_DataType::i16: os << "short";          break;
		case e_DataType::u16: os << "unsigned short"; break;
		case e_DataType::i32: os << "int";            break;
		case e_DataType::u32: os << "unsigned int";   break;
		case e_DataType::f32: os << "float";          break;
		case e_DataType::f64: os << "double";         break;
		default:
			os << "[UNKNOWN]";
		}

		return os;
	}

	GLenum to_GLenum(e_DataType t) {
		return static_cast<GLenum>(t);
	}
}