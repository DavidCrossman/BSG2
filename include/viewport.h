#pragma once

#include <memory>
#include "camera.h"

namespace bsg2 {
class Viewport {
public:
	float aspect_ratio;
	Viewport(std::unique_ptr<Camera> camera, int x, int y, int width, int height, float aspect_ratio);
	void apply();
	virtual void resize(int width, int height) = 0;
	inline int x() const { return m_x; }
	inline int y() const { return m_y; }
	inline int width() const { return m_width; }
	inline int height() const { return m_height; }
protected:
	std::unique_ptr<Camera> m_camera;
	int m_x, m_y, m_width, m_height;
};

//! A viewport that scales to fit the largest size possible that maintains its aspect ratio.
class FitViewport : public Viewport {
public:
	FitViewport(std::unique_ptr<OrthographicCamera> camera, int width, int height, float aspect_ratio);
	void resize(int width, int height) override;
	OrthographicCamera& camera();
};
}