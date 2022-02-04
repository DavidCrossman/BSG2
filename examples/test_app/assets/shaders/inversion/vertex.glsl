#version 330 core

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec4 v_colour;
layout(location = 2) in vec2 v_tex_coords;

out vec3 f_pos;
out vec4 f_colour;
out vec2 f_tex_coords;

uniform mat4 combined;

void main() {
	f_pos = v_pos;
	f_colour = v_colour;
	f_tex_coords = v_tex_coords;
	gl_Position = combined * vec4(v_pos, 1.0);
}