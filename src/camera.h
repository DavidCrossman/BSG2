#pragma once
#include <glm/glm.hpp>

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
	float rotation, zoom;
	OrthographicCamera(glm::vec2 pos = glm::vec2(0), float rotation = 0, float zoom = 1);
	void update() override;
};