#include "test_app.h"

#include <memory>

using glm::vec2, glm::vec4;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    TestApp* app = (TestApp*)glfwGetWindowUserPointer(window);
    app->resize(width, height);
}

TestApp::TestApp(const bsg2::WindowConfiguration& config) : Application(config),
        batch(&shaders.load_get("base")), squares(textures.load_get("squares.png")),
        view(std::make_unique<bsg2::OrthographicCamera>(), config.width, config.height, 100, 100) {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

TestApp::~TestApp() {}

void TestApp::resize(int width, int height) {
    view.update(width, height);
}

void TestApp::frame() {
    view.camera().rotation -= .01f;
    view.camera().update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    batch.combined = glm::mat4(1);
    batch.begin();
    batch.set_texture(squares);
    batch.draw_rect({ vec2(-1), vec4(.4f, .4f, .4f, 1), vec2(0) }, { vec2(1), vec4(.4f, .4f, .4f, 1), vec2(0) });
    batch.end();

    batch.combined = view.camera().combined();
    batch.begin();
    batch.set_texture(squares);
    batch.draw_rect({ vec2(0, 0), vec4(1), vec2(0, 0) }, { vec2(100, 100), vec4(1), vec2(1, 1) });
    batch.end();
}
