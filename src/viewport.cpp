#include "viewport.h"

#include <GL/glew.h>

namespace bsg2 {
Viewport::Viewport(std::unique_ptr<Camera> camera, int x, int y, int width, int height, float world_width, float world_height) :
		m_camera(std::move(camera)), world_width(world_width), world_height(world_height),
		m_x(x), m_y(y), m_width(width), m_height(height) {}

FitViewport::FitViewport(std::unique_ptr<OrthographicCamera> camera, int width, int height, float world_width, float world_height) :
		Viewport(std::move(camera), 0, 0, 0, 0, world_width, world_height) {
	update(width, height);
}

void FitViewport::update(int width, int height) {
	if (width * world_height > height * world_width) {
		m_x = int((width - height * world_width / world_height) / 2);
		m_y = 0;
		m_width = int(height * world_width / world_height);
		m_height = height;
	}
	else {
		m_x = 0;
		m_y = int((height - width * world_height / world_width) / 2);
		m_width = width;
		m_height = int(width * world_height / world_width);
	}

	m_camera->aspect_ratio = world_width / world_height;
	m_camera->update();

	glViewport(m_x, m_y, m_width, m_height);
}

OrthographicCamera& FitViewport::camera() {
	return *dynamic_cast<OrthographicCamera*>(m_camera.get());
}
}