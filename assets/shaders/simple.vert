#version 330 core

uniform mat4 MVP;

in vec3 vertex_color;
in vec3 vertex_position;
out vec3 color;

void main() {
    gl_Position = MVP * vec4(vertex_position, 1.0);
    color       = vertex_color;
}
