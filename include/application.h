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
		fullscreen_monitor(nullptr), max_delta(0.05f) {}
	int width, height, min_width, min_height, max_width, max_height, frame_time_ms, gl_version_major, gl_version_minor, msaa_samples;
	float max_delta;
	const char* name;
	GLFWmonitor* fullscreen_monitor;
};

class Application {
protected:
	GLFWwindow* window;
	float fps;
public:
	Application(GLFWwindow* window);
	virtual ~Application();
	virtual void frame(int frame_count, float delta) = 0;
	friend void execute(std::function<Application* (GLFWwindow*)> create_application, WindowConfiguration config);
};

void execute(std::function<Application* (GLFWwindow*)> create_application, WindowConfiguration config = WindowConfiguration());
}