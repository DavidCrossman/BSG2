#pragma once
#include <glm/glm.hpp>

namespace bsg2 {
struct Vertex {
	glm::vec3 pos;
	glm::vec4 colour;
	glm::vec2 tex_coords;
};
}