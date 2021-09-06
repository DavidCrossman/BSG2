#pragma once

#include <glm/glm.hpp>

namespace bsg2 {
class Camera {
protected:
	glm::mat4 m_projection, m_view, m_combined;
public:
	float aspect_ratio;
	Camera();
	virtual ~Camera() = 0;
	virtual void update();
	inline const glm::mat4& projection() const { return m_projection; }
	inline const glm::mat4& view() const { return m_view; }
	inline const glm::mat4& combined() const { return m_combined; }
};

class OrthographicCamera : public Camera {
public:
	glm::vec2 pos;
	float rotation, zoom, max_depth;
	OrthographicCamera(glm::vec2 pos = glm::vec2(0), float rotation = 0, float zoom = 1, float max_depth = 1000);
	void update() override;
};
}