#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec4 a_colour;
layout(location = 2) in vec2 a_tex_coords;

out vec3 v_pos;
out vec4 v_colour;
out vec2 v_tex_coords;

uniform mat4 combined;

void main() {
	v_pos = a_pos;
	v_colour = a_colour;
	v_tex_coords = a_tex_coords;
	gl_Position = combined * vec4(a_pos, 1.0);
}