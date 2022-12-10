#pragma once

#include "buffer_access.h"

#include <cstdint>

namespace glarses::opengl::buffers {
	// NOTE not 100% sure about the access specifiers, might be unneeded
	//
	class ShaderStorageBuffer {
	public:
		ShaderStorageBuffer() = default;
		~ShaderStorageBuffer();

		ShaderStorageBuffer             (const ShaderStorageBuffer&) = delete;
		ShaderStorageBuffer& operator = (const ShaderStorageBuffer&) = delete;
		ShaderStorageBuffer             (ShaderStorageBuffer&& ssb) noexcept;
		ShaderStorageBuffer& operator = (ShaderStorageBuffer&& sbb) noexcept;

		explicit ShaderStorageBuffer(e_BufferAccess access); // NOTE this will keep GL_SHADER_STORAGE_BUFFER bound

		uint32_t get_handle() const;
		uint32_t get_binding_point() const;

		void bind(uint32_t binding_point = 0);
		void unbind();

		void send_data(void* data, size_t num_bytes);

	private:
		uint32_t m_Handle       = 0;
		uint32_t m_BindingPoint = 0;
	};
}
