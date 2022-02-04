#version 330 core

in vec3 f_pos;
in vec4 f_colour;
in vec2 f_tex_coords;

out vec4 colour;

uniform sampler2D tex;

void main() {
	colour = f_colour * texture(tex, f_tex_coords);
	colour = vec4(1 - colour.rgb, colour.a);
}