#pragma once

#include "dependencies.h"

// Programmatically create an openGL cube using a VAO (vertex array object)
// currently the vertex format is 
//	attribute(0): xyx (float), non-normalized positions
//  attribute(1): rgb (float), non-normalized colors
class Cube {
public:
	Cube() = default; // allow a default-constructed cube without a valid openGL context
	~Cube();

	Cube             (const Cube&) = delete;
	Cube& operator = (const Cube&) = delete;
	Cube             (Cube&&) noexcept = default;
	Cube& operator = (Cube&&) noexcept = default;

	void init(); // this requires an active openGL context with core support -- only create one of these after the window is up and running
	void draw(); // only valid after calling init();

private:
	GLuint m_VAO          = 0;
	GLuint m_VertexBuffer = 0; // aka VBO
	GLuint m_ColorBuffer  = 0;

	static constexpr size_t k_NumIndices = 6 * 2 * 3; // 6 sides of a cube, 2 triangles per side, 3 vertices per triangle
};