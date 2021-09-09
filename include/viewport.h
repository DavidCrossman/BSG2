#pragma once

#include <memory>
#include "camera.h"

namespace bsg2 {
class Viewport {
protected:
	std::unique_ptr<Camera> m_camera;
	int m_x, m_y, m_width, m_height;
public:
	float world_width, world_height;
	Viewport(std::unique_ptr<Camera> camera, int x, int y, int width, int height, float world_width, float world_height);
	virtual void update(int width, int height) = 0;
	inline int x() const { return m_x; }
	inline int y() const { return m_y; }
	inline int width() const { return m_width; }
	inline int height() const { return m_height; }
};

class FitViewport : public Viewport {
public:
	FitViewport(std::unique_ptr<OrthographicCamera> camera, int width, int height, float world_width, float world_height);
	void update(int width, int height) override;
	OrthographicCamera& camera();
};
}