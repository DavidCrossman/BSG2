#include "test_app.h"

#include <memory>

using glm::vec2, glm::vec4;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    TestApp* app = (TestApp*)glfwGetWindowUserPointer(window);
    app->resize(width, height);
}

TestApp::TestApp(const bsg2::ApplicationConfiguration& config) : Application(config),
        textures("examples/test_app/assets/textures/"), shaders("examples/test_app/assets/shaders/"),
        batch(&shaders.load_get("base")), squares(textures.load_get("squares.png")),
        view(std::make_unique<bsg2::OrthographicCamera>(), config.width, config.height, 1080/720.f) {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    srand(12);
    for (int i = 0; i < 20; i++) {
        vertices.push_back(bsg2::Vertex(vec2(i / 20.f, (rand() % 1000) / 2000.f), vec4(i / 20.f, i / 20.f, i / 20.f, 1), vec2(0)));
    }
}

TestApp::~TestApp() {}

void TestApp::resize(int width, int height) {
    view.update(width, height);
}

void TestApp::frame() {
    //view.camera().rotation -= .01f;
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
    batch.draw_tri_strip({ bsg2::Vertex(vec2(0), vec4(1), vec2(1)), bsg2::Vertex(vec2(0, 1), vec4(1), vec2(1)), bsg2::Vertex(vec2(1), vec4(1), vec2(1)) });
    batch.draw_tri_strip(vertices);
    batch.draw_rect(vec2(-0.2f), 0.2f, 0.3f, 3.14159265f * frame_count() / 200);
    batch.end();
}
