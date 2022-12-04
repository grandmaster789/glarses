#pragma once

#include "../dependencies.h"
#include <span>

namespace glarses::opengl {
	class DataBuffer {
	public:
		DataBuffer();
		~DataBuffer();

		DataBuffer             (const DataBuffer&) = delete;
		DataBuffer& operator = (const DataBuffer&) = delete;
		DataBuffer             (DataBuffer&& db) noexcept;
		DataBuffer& operator = (DataBuffer&& db) noexcept;

		GLuint get_handle() const;

		// https://docs.gl/gl4/glBufferStorage 
		//
		template <typename T>
		void store(
			std::span<T> data, 
			GLbitfield   flags = 0 // GL_DYNAMIC_STORAGE_BIT, GL_MAP_READ_BIT, GL_MAP_WRITE_BIT, GL_MAP_PERSISTENT_BIT, GL_MAP_COHERENT_BIT, and/or GL_CLIENT_STORAGE_BIT
		);

	private:
		GLuint m_Handle = 0;
	};
}

#include "data_buffer.inl"