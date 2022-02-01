#include "camera.h"

#include <glm/gtx/transform.hpp>

using glm::vec2, glm::vec3;

namespace bsg2 {
Camera::Camera() : m_projection(1.0), m_view(1.0), m_combined(1.0), aspect_ratio(1) {
	update();
}

Camera::~Camera() {}

void Camera::update() {
	m_combined = m_projection * m_view;
}

OrthographicCamera::OrthographicCamera(vec2 pos, float rotation, float zoom, float max_depth) :
		Camera(), pos(pos), rotation(rotation), zoom(zoom), max_depth(max_depth) {
	update();
}

void OrthographicCamera::update() {
	rotation = std::fmodf(rotation, glm::pi<float>() * 2);
	if (rotation < 0) rotation += 2 * glm::pi<float>();

	m_view = glm::rotate(rotation, vec3(0, 0, -1)) * glm::translate(vec3(-pos, 0));
	m_projection = glm::ortho(-zoom * aspect_ratio, zoom * aspect_ratio, -zoom, zoom, -max_depth, max_depth);

	Camera::update();
}
}