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

glm::mat3 Application::screen_to_world() {
    return glm::mat3(2.0 / width, 0, 0, 0, -2.0 / height, 0, -1, 1, 1);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);

    Application *app = (Application *)glfwGetWindowUserPointer(window);
    app->width = width;
    app->height = height;
    app->cam.aspect_ratio = width / (float)height;
}

Application::Application(int width, int height) : width(width), height(height) {
    glewExperimental = true;
    if (!glfwInit()) {
        std::cerr << "ERROR: Could not initialise GLFW" << std::endl;
        std::exit(1);
    }

    cam = OrthographicCamera();
    cam.aspect_ratio = width / (float)height;
    cam.update();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "BSG2", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        std::cerr << "ERROR: Could not create GLFWwindow" << std::endl;
        std::exit(2);
    }

    glfwSetWindowSizeLimits(window, 320, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);

    int glewInitResult = glewInit();
    if (glewInitResult) {
        glfwTerminate();
        std::cerr << "ERROR: Could not initialise GLEW, error code " << glewInitResult << std::endl;
        std::exit(3);
    }

    initialise_asset_managers();
}

Application::~Application() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::mainloop() {
    SleepManager sleep_manager;//

    constexpr int FRAMES_FOR_FPS = 100;//
    constexpr std::chrono::duration<double> MIN_FRAME_TIME = std::chrono::milliseconds(5);//
    unsigned long long frame_count = 0;//
    double fps = 0;//
    auto last_frame = std::chrono::high_resolution_clock::now();//
    auto last_fps_update = std::chrono::high_resolution_clock::now();//

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    TextureManager textures;
    Texture *squares = textures.load_get("squares.png");
    ShaderManager shaders;

    Batch batch(shaders.load_get("base"));

    bool window_should_close = false;
    while (!window_should_close) {
        frame_count++;//

        auto now = std::chrono::high_resolution_clock::now();//
        auto end = now + MIN_FRAME_TIME;//
        float delta = std::chrono::duration_cast<std::chrono::microseconds>(now - last_frame).count() * 0.000001f;//
        last_frame = now;//

        if (frame_count % FRAMES_FOR_FPS == 0) {//
            std::chrono::duration<double> time_since_fps_update = now - last_fps_update;//
            last_fps_update = now;//
            fps = FRAMES_FOR_FPS / time_since_fps_update.count();//
        }//

        glfwPollEvents();//

        cam.update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        batch.combined = cam.combined;
        batch.begin();
        batch.set_texture(squares);
        batch.draw_rect({ vec3(0.1, 0.1, 0), vec4(1), vec2(0, 0) }, { vec3(0.5, 0.5, 0), vec4(1), vec2(1, 1) });
        batch.end();

        glFinish();
        glfwSwapBuffers(window);

        if (glfwWindowShouldClose(window)) window_should_close = true;//

        int sleep_duration = (int)std::chrono::duration_cast<std::chrono::milliseconds>(end - std::chrono::high_resolution_clock::now()).count();//
        if (sleep_duration > 0) sleep_manager.sleep(sleep_duration);//
    }
}
