#version 430

layout(location = 0) in vec2 position;

out vec2 tex;

void main() {
    gl_Position = vec4(position, 0, 1);
    tex = (position + 1) * 0.5;
}
