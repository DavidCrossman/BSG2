#version 330 core

in vec3 v_pos;
in vec4 v_colour;
in vec2 v_tex_coords;

out vec4 f_colour;

uniform sampler2D tex;

void main() {
	f_colour = v_colour * texture(tex, v_tex_coords);
	f_colour.rgb = 1 - f_colour.rgb;
}