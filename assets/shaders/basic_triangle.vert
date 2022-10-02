#version 460 core

layout(std140, binding = 0) 
uniform PerFrameData {
	// we require a 4x4 matrix to be set for the entire frame
	// this can be set via glNamedBufferSubData()
	//
	uniform mat4 g_ModelViewProjection; 
};

layout (location = 0) 
out vec2 vtx_uv; // this shader will output a texture coordinate; in the fragment shader this will be interpolated between vertices

// directly embed vertex data in the shader
const vec2 pos[3] = vec2[3](
	vec2(-0.6f, -0.4f),
	vec2( 0.6f, -0.4f),
	vec2( 0.0f,  0.6f)
);

// directly embed texture coordinates in the shader
const vec2 tex_coords[3] = vec2[3](
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),
	vec2(0.5f, 1.0f)
);

void main() {
	// lookup vertex information from the ID
	// ie. this should be called with glDrawArrays(GL_TRIANGLES, 0, 3);
	// 
	gl_Position = g_ModelViewProjection * vec4(pos[gl_VertexID], 0.0f, 1.0f);
	vtx_uv      = tex_coords[gl_VertexID];
}