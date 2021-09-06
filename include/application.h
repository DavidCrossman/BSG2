#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <vector>
#include "camera.h"

namespace bsg2 {
struct WindowConfiguration {
	WindowConfiguration() : width(1024), height(768), min_width(320), min_height(200), max_width(GLFW_DONT_CARE), max_height(GLFW_DONT_CARE),
		frame_time_ms(16), name("Window"), gl_version_major(3), gl_version_minor(3), msaa_samples(4),
		fullscreen_monitor(nullptr), max_delta(0.05f), icon_path("") {}
	int width, height, min_width, min_height, max_width, max_height, frame_time_ms, gl_version_major, gl_version_minor, msaa_samples;
	float max_delta;
	const char *name, *icon_path;
	GLFWmonitor* fullscreen_monitor;
};

class Application {
	float m_delta, m_fps;
	unsigned long long m_frame_count;
	const float MAX_DELTA;
	const int FRAME_TIME;
protected:
	GLFWwindow* const window;
	virtual void frame() = 0;
public:
	Application(const WindowConfiguration& config);
	Application(const Application& other) = delete;
	virtual ~Application();
	void run();
	inline float delta() const { return m_delta; }
	inline float fps() const { return m_fps; }
	inline unsigned long long frame_count() const { return m_frame_count; }
};
}