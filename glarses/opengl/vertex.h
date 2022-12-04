#pragma once

#include "../dependencies.h"

#include <cstddef>

namespace glarses::opengl {
	class DataBuffer;

	struct VertexMember {
		GLint     m_Size;
		GLenum    m_Type;
		GLboolean m_IsNormalized;
		GLsizei   m_Stride;
		GLuint    m_Offset;
	};

	// for now, we'll just use a single vertex format
	struct Vertex {
		glm::vec3 m_Position;
		glm::vec3 m_Color; // ~ reasonable for trivial models, should probably be replaced with texcoords

		static const VertexMember s_PositionDesc;
		static const VertexMember s_ColorDesc;
	};

	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		VertexArray             (const VertexArray&) = delete;
		VertexArray& operator = (const VertexArray&) = delete;
		VertexArray             (VertexArray&& va) noexcept;
		VertexArray& operator = (VertexArray&& va) noexcept;

		GLuint get_handle() const;
		void   bind()       const;

		void attach(
			const DataBuffer&   db,
			GLuint              index,
			const VertexMember& desc
		);

	private:
		GLuint m_Handle = 0;
	};
}
