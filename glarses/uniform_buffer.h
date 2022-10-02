#pragma once

#include "dependencies.h"

// NOTE currently you must manually do pointer arithmetic to work with uniform buffers;
//      at some point this should be abstracted more cleanly
//			- boost::pfr?
//          - some kind of typelist metaprogramming?
//          - other reflection magic?
class UniformBuffer {
public:
	UniformBuffer() = default;
	~UniformBuffer();

	UniformBuffer             (const UniformBuffer&) = delete;
	UniformBuffer& operator = (const UniformBuffer&) = delete;
	UniformBuffer             (UniformBuffer&& u) noexcept;
	UniformBuffer& operator = (UniformBuffer&& u) noexcept; 

	template <typename T>
	static UniformBuffer create(GLuint binding_point);

	template <typename T>
	void set_data(const T& data);

	void bind();

private:
	GLuint     m_Handle       = 0;
	GLuint     m_BindingPoint = 0; // this index will end up being the layout binding number in the GLSL code
	GLsizeiptr m_BufferSize   = 0;
};

#include "uniform_buffer.inl"