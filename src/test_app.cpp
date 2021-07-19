#include "test_app.h"

using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    TestApp* app = (TestApp*)glfwGetWindowUserPointer(window);
    app->resize(width, height);
}

TestApp::TestApp(GLFWwindow* window, int width, int height) : Application(window), width(width), height(height), batch(new bsg2::Batch(shaders.load_get("base"))),
                                       squares(textures.load_get("squares.png")) {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    cam.aspect_ratio = width / (float)height;
    cam.update();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

TestApp::~TestApp() {
    delete batch;
}

void TestApp::resize(int width, int height) {
    this->width = width;
    this->height = height;
    cam.aspect_ratio = width / (float)height;
}

void TestApp::frame(int frame_count, float delta) {
    cam.update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    batch->combined = cam.combined;
    batch->begin();
    batch->set_texture(squares);
    batch->draw_rect({ vec3(0.1, 0.1, 0), vec4(1), vec2(0, 0) }, { vec3(0.5, 0.5, 0), vec4(1), vec2(1, 1) });
    batch->end();
}

mat3 TestApp::screen_to_world() {
    return mat3(2.0 / width, 0, 0, 0, -2.0 / height, 0, -1, 1, 1);
}