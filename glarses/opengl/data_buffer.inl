#pragma once

#include "data_buffer.h"

namespace glarses::opengl {
	template <typename T>
	void DataBuffer::store(
		std::span<T> data,
		GLbitfield   flags
	) {
		glNamedBufferStorage(
			m_Handle,
			data.size_bytes(),
			data.data(),
			flags
		);
	}
}