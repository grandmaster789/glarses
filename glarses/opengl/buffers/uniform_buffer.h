#pragma once

#include <cstdint>
#include <functional>
#include <string>

#include "buffer_access.h"

namespace glarses::opengl::resources {
	class Shader;
}

namespace glarses::opengl::buffers {
	class UniformBuffer {
	public:
		UniformBuffer() = default;
		~UniformBuffer();

		UniformBuffer(
			size_t         num_bytes,
			uint32_t       binding_point = 0,
			uint32_t       offset        = 0,
			e_BufferAccess access        = e_BufferAccess::dynamic_draw
		);

		UniformBuffer             (const UniformBuffer&) = delete;
		UniformBuffer& operator = (const UniformBuffer&) = delete;
		UniformBuffer             (UniformBuffer&& ub) noexcept;
		UniformBuffer& operator = (UniformBuffer&& ub) noexcept;

		uint32_t get_handle() const;
		
		void bind();
		void unbind();

		template <typename T>
		void set_subdata(
			const T& data, 
			size_t   offset
		);

		template <typename T>
		void set_subdata(
			const T&                       data, 
			std::reference_wrapper<size_t> offset_inout // tracks the current stride of the data
		);

		static void bind_to_shader(
			resources::Shader& shader,
			uint32_t           location, 
			uint32_t           binding_point = 0
		);

		static void bind_to_shader(
			resources::Shader& shader,
			const std::string& location_name, 
			uint32_t           binding_point = 0
		);

		static uint32_t get_location(
			const resources::Shader& shader,
			const std::string&       location_name
		);

	private:
		uint32_t m_Handle = 0;
	};
}

#include "uniform_buffer.inl"