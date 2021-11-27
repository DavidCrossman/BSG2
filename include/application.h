#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "camera.h"

namespace bsg2 {
struct ApplicationConfiguration {
	int width = 1024, height = 768, min_width = 320, min_height = 200, max_width = GLFW_DONT_CARE,
		max_height = GLFW_DONT_CARE, frame_time_ms = 16, gl_version_major = 3, gl_version_minor = 3, msaa_samples = 4;
	float max_delta = 0.05f;
	const char* name = "Window";
	std::string icon_path = "";
	GLFWmonitor* fullscreen_monitor = nullptr;
	GLFWwindow* share_window = nullptr;
};

/**
* The base application class that manages its GLFWwindow pointer and repeatedly
* calls its frame() method until it is destroyed.
*
* Extend this to create your own application.
*/
class Application {
public:
	Application(const ApplicationConfiguration& config);
	Application(const Application& other) = delete;
	virtual ~Application();
	//! Begins the application loop.
	void run();
	//! \return The number of seconds the last frame() call took.
	inline float delta() const { return m_delta; }
	//! \return The average number of times frame() is called per second.
	inline float fps() const { return m_fps; }
	//! \return The total number of times frame() has been called since the application began.
	inline unsigned long long frame_count() const { return m_frame_count; }
protected:
	GLFWwindow* const window;
	//! A routinely called method. Put rendering and update code here.
	virtual void frame() = 0;
private:
	float m_delta, m_fps;
	unsigned long long m_frame_count;
	const float MAX_DELTA;
	const int FRAME_TIME;
};
}