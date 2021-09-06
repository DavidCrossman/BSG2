#include "test_app.h"

using glm::vec2, glm::vec4;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    TestApp* app = (TestApp*)glfwGetWindowUserPointer(window);
    app->resize(width, height);
}

TestApp::TestApp(const bsg2::WindowConfiguration& config) : Application(config),
        batch(&shaders.load_get("base")), squares(textures.load_get("squares.png")) {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    cam.aspect_ratio = config.width / (float)config.height;
    cam.update();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

TestApp::~TestApp() {}

void TestApp::resize(int width, int height) {
    cam.aspect_ratio = width / (float)height;
}

void TestApp::frame() {
    cam.update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    batch.combined = cam.combined();
    batch.begin();
    batch.set_texture(squares);
    batch.draw_rect({ vec2(0.1, 0.1), vec4(1), vec2(0, 0) }, { vec2(0.5, 0.5), vec4(1), vec2(1, 1) });
    batch.end();
}
