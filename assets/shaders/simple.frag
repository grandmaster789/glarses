#version 330 core

in vec3 color;
out vec4 out_fragment;

void main() {
    out_fragment = vec4(color, 1.0);
}
