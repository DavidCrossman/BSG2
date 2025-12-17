#pragma once

#include <glm/glm.hpp>

namespace bsg2 {
//! A structure to store the position, colour, texture coordinates and depth of a vertex.
struct Vertex {
	inline Vertex(glm::vec2 pos, glm::vec4 colour, glm::vec2 tex_coords, float depth = 0) : 
		pos(pos), colour(colour), tex_coords(tex_coords), depth(depth) {}
	inline Vertex() {}
	glm::vec2 pos;
	float depth;
	glm::vec4 colour;
	glm::vec2 tex_coords;
	inline bool operator==(const Vertex& rhs) const {
		return pos == rhs.pos && depth == rhs.depth && colour == rhs.colour && tex_coords == rhs.tex_coords;
	}
};
}