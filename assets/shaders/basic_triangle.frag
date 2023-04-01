#version 460 core

layout (location = 0) 
in vec2 vtx_uv; // expect a vec2 at input location 0

layout (location = 0) 
out vec4 out_FragColor; // we'll output a vec4 at output location 0

uniform sampler2D texture0; // we're going to sample from a texture
void main() {
	out_FragColor = texture(texture0, vtx_uv); // https://registry.khronos.org/OpenGL-Refpages/gl4/html/texture.xhtml
}