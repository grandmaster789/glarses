#pragma once

// some of these headers *require* a strict include order, so this file makes sure everything plays nicely

#include <gl/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// consider all angles radians -- none of that 'degree' stuff
// use metaprogramming helpers (number of elements in a vec etc)
// tbh glm isn't my favorite but it's a reasonable option
#define GLM_FORCE_RADIANS
#define GLM_META_PROG_HELPERS
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/quaternion.hpp>
	#include <glm/gtc/type_ptr.hpp>
	#include <glm/gtx/transform2.hpp>
	#include <glm/ext.hpp>
#undef GLM_META_PROG_HELPERS
#undef GLM_FORCE_RADIANS

#include "stb_image.h"

#include <iosfwd>

// additional ostream operators for glm
namespace glm {
	std::ostream& operator << (std::ostream& os, const vec2& v);
	std::ostream& operator << (std::ostream& os, const vec3& v);
	std::ostream& operator << (std::ostream& os, const vec4& v);

	std::ostream& operator << (std::ostream& os, const mat2& m);
	std::ostream& operator << (std::ostream& os, const mat3& m);
	std::ostream& operator << (std::ostream& os, const mat4& m);
}