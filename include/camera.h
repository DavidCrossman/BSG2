#pragma once

#include <glm/glm.hpp>

namespace bsg2 {
class Camera {
public:
	glm::mat4 projection, view, combined;
	float aspect_ratio;
	Camera();
	virtual void update();
};

class OrthographicCamera : public Camera {
public:
	glm::vec2 pos;
	float rotation, zoom, max_depth;
	OrthographicCamera(glm::vec2 pos = glm::vec2(0), float rotation = 0, float zoom = 1, float max_depth = 1000);
	void update() override;
};
}