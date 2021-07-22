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
#include "texture_manager.h"
#include "vertex.h"

using namespace glm;

namespace bsg2 {

Application::Application(GLFWwindow* window) : window(window), fps(-1) {}

Application::~Application() {}

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

    if (config.icon_path != "") {
        GLFWimage icons[1];
        icons[0].pixels = SOIL_load_image((get_asset_dir() + "textures/" + config.icon_path).c_str(), &icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
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

void execute(std::function<Application* (GLFWwindow*)> create_application, WindowConfiguration config) {
    initialise_asset_managers();

    GLFWwindow* window = create_window(config);

    Application* app = create_application(window);
    glfwSetWindowUserPointer(window, app);

    SleepManager sleep_manager;
    constexpr int FRAMES_FOR_FPS = 100;
    const float MAX_DELTA = config.max_delta;
    std::chrono::duration<double> min_frame_time = std::chrono::milliseconds(config.frame_time_ms);
    unsigned long long frame_count = 0;
    auto last_frame = std::chrono::high_resolution_clock::now();
    auto last_fps_update = std::chrono::high_resolution_clock::now();

    bool window_should_close = false;
    while (!window_should_close) {
        frame_count++;

        auto now = std::chrono::high_resolution_clock::now();
        auto end = now + min_frame_time;
        auto diff = now - last_frame;
        last_frame = now;

        if (frame_count % FRAMES_FOR_FPS == 0) {
            std::chrono::duration<double> time_since_fps_update = now - last_fps_update;
            last_fps_update = now;
            app->fps = FRAMES_FOR_FPS / time_since_fps_update.count();
        }

        glfwPollEvents();

        app->frame(frame_count, std::fminf(MAX_DELTA, std::chrono::duration_cast<std::chrono::microseconds>(diff).count() * 0.000001f));

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