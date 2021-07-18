#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <vector>

#include "camera.h"

namespace bsg2 {
class Application {
public:
	Application();
	virtual ~Application();
	virtual void frame(int frame_count, float delta) = 0;
	virtual void framebuffer_size_callback(int width, int height);
};

struct WindowConfiguration {
	WindowConfiguration() : width(1024), height(768), min_width(320), min_height(200), max_width(GLFW_DONT_CARE), max_height(GLFW_DONT_CARE),
							frame_time_ms(16), name("Window"), gl_version_major(3), gl_version_minor(3), msaa_samples(4),
							fullscreen_monitor(nullptr) {}
	int width, height, min_width, min_height, max_width, max_height, frame_time_ms, gl_version_major, gl_version_minor, msaa_samples;
	const char* name;
	GLFWmonitor* fullscreen_monitor;
};

void execute(std::function<Application* ()> create_application, WindowConfiguration config = WindowConfiguration());
}