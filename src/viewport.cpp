#include "viewport.h"

#include <GL/glew.h>

namespace bsg2 {
Viewport::Viewport(std::unique_ptr<Camera> camera, int x, int y, int width, int height, float aspect_ratio) :
		m_camera(std::move(camera)), aspect_ratio(aspect_ratio), m_x(x), m_y(y), m_width(width), m_height(height) {}

FitViewport::FitViewport(std::unique_ptr<OrthographicCamera> camera, int width, int height, float aspect_ratio) :
		Viewport(std::move(camera), 0, 0, 0, 0, aspect_ratio) {
	update(width, height);
}

void FitViewport::update(int width, int height) {
	if (width > height * aspect_ratio) {
		m_x = int((width - height * aspect_ratio) / 2);
		m_y = 0;
		m_width = int(height * aspect_ratio);
		m_height = height;
	}
	else {
		m_x = 0;
		m_y = int((height - width / aspect_ratio) / 2);
		m_width = width;
		m_height = int(width / aspect_ratio);
	}

	m_camera->aspect_ratio = aspect_ratio;
	m_camera->update();

	glViewport(m_x, m_y, m_width, m_height);
}

OrthographicCamera& FitViewport::camera() {
	return *dynamic_cast<OrthographicCamera*>(m_camera.get());
}
}