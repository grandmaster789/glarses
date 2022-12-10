#pragma once

namespace glarses::opengl::resources {
	class Shader {
	public:

		uint32_t get_handle() const;

	private:
		uint32_t m_Handle = 0;
	};
}