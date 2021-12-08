#include "application.h"

#include <chrono>
#include <cstdlib>
#include <glm/glm.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <SOIL2/SOIL2.h>
#include "batch.h"
#include "shader.h"
#include "sleep_util.h"
#include "vertex.h"

namespace bsg2 {
GLFWwindow* const create_window(const ApplicationConfiguration& config) {
    glewExperimental = true;
    if (!glfwInit()) {
        std::cerr << "ERROR: Could not initialise GLFW" << std::endl;
        std::exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.gl_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.gl_version_minor);
    glfwWindowHint(GLFW_SAMPLES, config.msaa_samples);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* const window = glfwCreateWindow(config.width, config.height, config.name, config.fullscreen_monitor, config.share_window);

    if (!window) {
        glfwTerminate();
        std::cerr << "ERROR: Could not create GLFWwindow" << std::endl;
        std::exit(2);
    }

    glfwSetWindowSizeLimits(window, config.min_width, config.min_height, config.max_width, config.max_height);

    if (!config.icon_path.empty()) {
        GLFWimage icons[1];
        std::size_t index = config.icon_path.find_last_of('/') + 1;
        std::string dir = find_full_dir(config.icon_path.substr(0, index)) + config.icon_path.substr(index);
        icons[0].pixels = SOIL_load_image(dir.c_str(), &icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
        glfwSetWindowIcon(window, 1, icons);
        SOIL_free_image_data(icons[0].pixels);
    }

    glfwMakeContextCurrent(window);

    int glewInitResult = glewInit();
    if (glewInitResult) {
        glfwTerminate();
        std::cerr << "ERROR: Could not initialise GLEW, error code " << glewInitResult << std::endl;
        std::exit(3);
    } 

    return window;
}

Application::Application(const ApplicationConfiguration& config) : window(create_window(config)), m_should_exit(false), m_fps(-1),
        MAX_DELTA(config.max_delta), FRAME_TIME(config.frame_time_ms), m_frame_count(0), m_delta(-1) {
    glfwSetWindowUserPointer(window, this);
}

Application::~Application() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::run() {
    SleepManager sleep_manager;
    constexpr int FRAMES_FOR_FPS = 100;
    std::chrono::duration<double> min_frame_time = std::chrono::milliseconds(FRAME_TIME);
    auto last_frame = std::chrono::high_resolution_clock::now();
    auto last_fps_update = std::chrono::high_resolution_clock::now();

    while (!m_should_exit) {
        m_frame_count++;

        auto now = std::chrono::high_resolution_clock::now();
        auto end = now + min_frame_time;
        auto diff = now - last_frame;
        last_frame = now;

        if (m_frame_count % FRAMES_FOR_FPS == 0) {
            std::chrono::duration<double> time_since_fps_update = now - last_fps_update;
            last_fps_update = now;
            m_fps = FRAMES_FOR_FPS / (float)time_since_fps_update.count();
        }

        glfwPollEvents();

        m_delta = std::fminf(MAX_DELTA, std::chrono::duration_cast<std::chrono::microseconds>(diff).count() * 0.000001f);
        frame();

        glFinish();
        glfwSwapBuffers(window);

        if (glfwWindowShouldClose(window)) m_should_exit = true;

        int sleep_duration = (int)std::chrono::duration_cast<std::chrono::milliseconds>(end - std::chrono::high_resolution_clock::now()).count();
        if (sleep_duration > 0) sleep_manager.sleep(sleep_duration);
    }
}

void Application::exit() {
    m_should_exit = true;
}
}