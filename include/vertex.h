#pragma once

#include <glm/glm.hpp>

namespace bsg2 {
//! A structure to store the position, colour, texture coordinates and depth of a vertex.
struct Vertex {
	inline Vertex(glm::vec2 pos, glm::vec4 colour, glm::vec2 tex_coords, float depth = 0) : 
		pos(pos), colour(colour), tex_coords(tex_coords), depth(depth) {}
	glm::vec2 pos;
	glm::vec4 colour;
	glm::vec2 tex_coords;
	float depth;
};
}