#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace bsg2 {
//! A structure to store the position, colour, texture coordinates and depth of a vertex.
struct Vertex {
	static float epsilon;
	Vertex(glm::vec2 pos, glm::vec4 colour, glm::vec2 tex_coords, float depth = 0);
	Vertex();
	glm::vec2 pos;
	float depth;
	glm::vec4 colour;
	glm::vec2 tex_coords;
	bool operator==(const Vertex& rhs) const;
};
}