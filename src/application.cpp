#include "application.h"

#include <chrono>
#include <cstdlib>
#include <glm/glm.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "batch.h"
#include "shader.h"
#include "sleep_util.h"
#include "texture_manager.h"
#include "vertex.h"

using namespace glm;

namespace bsg2 {
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);

    Application *app = (Application *)glfwGetWindowUserPointer(window);
    app->framebuffer_size_callback(width, height);
}

Application::Application() {}

Application::~Application() {}

void Application::framebuffer_size_callback(int width, int height) {}

GLFWwindow* create_window(WindowConfiguration config) {
    glewExperimental = true;
    if (!glfwInit()) {
        std::cerr << "ERROR: Could not initialise GLFW" << std::endl;
        std::exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.gl_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.gl_version_minor);
    glfwWindowHint(GLFW_SAMPLES, config.msaa_samples);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(config.width, config.height, config.name, config.fullscreen_monitor, nullptr);

    if (!window) {
        glfwTerminate();
        std::cerr << "ERROR: Could not create GLFWwindow" << std::endl;
        std::exit(2);
    }

    glfwSetWindowSizeLimits(window, config.min_width, config.min_height, config.max_width, config.max_height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);

    int glewInitResult = glewInit();
    if (glewInitResult) {
        glfwTerminate();
        std::cerr << "ERROR: Could not initialise GLEW, error code " << glewInitResult << std::endl;
        std::exit(3);
    }

    return window;
}

void execute(Application* app, WindowConfiguration config) {
    GLFWwindow* window = create_window(config);
    glfwSetWindowUserPointer(window, app);

    initialise_asset_managers();

    app->initialise();

    SleepManager sleep_manager;
    constexpr int FRAMES_FOR_FPS = 100;
    std::chrono::duration<double> min_frame_time = std::chrono::milliseconds(config.frame_time_ms);
    unsigned long long frame_count = 0;
    double fps = 0;
    auto last_frame = std::chrono::high_resolution_clock::now();
    auto last_fps_update = std::chrono::high_resolution_clock::now();

    bool window_should_close = false;
    while (!window_should_close) {
        frame_count++;

        auto now = std::chrono::high_resolution_clock::now();
        auto end = now + min_frame_time;
        last_frame = now;

        if (frame_count % FRAMES_FOR_FPS == 0) {
            std::chrono::duration<double> time_since_fps_update = now - last_fps_update;
            last_fps_update = now;
            fps = FRAMES_FOR_FPS / time_since_fps_update.count();
        }

        glfwPollEvents();

        app->frame(frame_count, std::chrono::duration_cast<std::chrono::microseconds>(now - last_frame).count() * 0.000001f);

        glFinish();
        glfwSwapBuffers(window);

        if (glfwWindowShouldClose(window)) window_should_close = true;

        int sleep_duration = (int)std::chrono::duration_cast<std::chrono::milliseconds>(end - std::chrono::high_resolution_clock::now()).count();
        if (sleep_duration > 0) sleep_manager.sleep(sleep_duration);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
}