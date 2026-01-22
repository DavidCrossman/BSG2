#include "vertex.h"

#include <limits>
#include <glm/gtc/epsilon.hpp>

namespace bsg2 {
float Vertex::epsilon = std::numeric_limits<float>::epsilon();

Vertex::Vertex(glm::vec2 pos, glm::vec4 colour, glm::vec2 tex_coords, float depth) :
    pos(pos), colour(colour), tex_coords(tex_coords), depth(depth) {}

Vertex::Vertex() : pos(0), colour(0), tex_coords(0), depth(0) {}

bool Vertex::operator==(const Vertex& rhs) const {
    return glm::all(glm::epsilonEqual(pos, rhs.pos, epsilon))
        && glm::all(glm::epsilonEqual(colour, rhs.colour, epsilon))
        && glm::all(glm::epsilonEqual(tex_coords, rhs.tex_coords, epsilon))
        && abs(depth - rhs.depth) < epsilon;
}
}