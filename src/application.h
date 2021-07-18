#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "camera.h"

class Application {
    GLFWwindow* window;

   public:
    OrthographicCamera cam;
    int width, height;

	glm::mat3 screen_to_world();

	Application(int width = 1024, int height = 768);
	~Application();
	void mainloop();
};