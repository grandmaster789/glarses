#include "dependencies.h"

#define STB_IMAGE_IMPLEMENTATION
	#include "stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION

#define STB_IMAGE_WRITE_IMPLEMENTATION
	#include "stb_image_write.h"
#undef STB_IMAGE_WRITE_IMPLEMENTATION

#include <ostream>
#include <string>
#include <format>

namespace glm {
	std::ostream& operator << (std::ostream& os, const vec2& v) {
		os << std::format("({}, {})", v.x, v.y);
		return os;
	}

	std::ostream& operator << (std::ostream& os, const vec3& v) {
		os << std::format("({}, {}, {})", v.x, v.y, v.z);
		return os;
	}

	std::ostream& operator << (std::ostream& os, const vec4& v) {
		os << std::format("({}, {}, {}, {})", v.x, v.y, v.z, v.w);
		return os;
	}

	std::ostream& operator << (std::ostream& os, const mat2& m) {
		os << "\n[" << m[0][0] << '\t' << m[1][0] << "]\n";
		os <<   '[' << m[0][1] << '\t' << m[1][1] << "]\n";
		return os;
	}

	std::ostream& operator << (std::ostream& os, const mat3& m) {
		os << "\n[" << m[0][0] << '\t' << m[1][0] << '\t' << m[2][0] << "]\n";
		os <<   '[' << m[0][1] << '\t' << m[1][1] << '\t' << m[2][1] << "]\n";
		os <<   '[' << m[0][2] << '\t' << m[1][2] << '\t' << m[2][2] << "]\n";
		return os;
	}

	std::ostream& operator << (std::ostream& os, const mat4& m) {
		os << "\n[" << m[0][0] << '\t' << m[1][0] << '\t' << m[2][0] << '\t' << m[3][0] << "]\n";
		os <<   '[' << m[0][1] << '\t' << m[1][1] << '\t' << m[2][1] << '\t' << m[3][1] << "]\n";
		os <<   '[' << m[0][2] << '\t' << m[1][2] << '\t' << m[2][2] << '\t' << m[3][2] << "]\n";
		os <<   '[' << m[0][3] << '\t' << m[1][3] << '\t' << m[2][3] << '\t' << m[3][3] << "]\n";
		return os;
	}
}