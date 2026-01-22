#include "test_app.h"

#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using glm::vec2, glm::vec3, glm::vec4;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    TestApp* app = (TestApp*)glfwGetWindowUserPointer(window);
    app->resize(width, height);
}

TestApp::TestApp(const bsg2::ApplicationConfiguration& config) : Application(config),
        textures("examples/test_app/assets/textures/"), shaders("examples/test_app/assets/shaders/"),
        inversion(shaders.load_get("inversion")), squares(textures.load_get("squares.png")),
        view(std::make_unique<bsg2::OrthographicCamera>(), config.width, config.height, 1080/720.f),
        frame_buffer(config.width, config.height, config.msaa_samples) {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    srand(12);
    for (int i = 0; i < 20; i++) {
        strip_vertices.push_back(bsg2::Vertex(vec2(i / 20.f, (rand() % 1000) / 2000.f), vec4(i / 20.f, i / 20.f, i / 20.f, 1), vec2(0)));
    }
    int vert_num = 100;
    for (int i = 0; i < vert_num; i++) {
        fan_vertices.push_back(bsg2::Vertex(vec2(cosf((i / (vert_num / 2.f)) * 3.14159265359f),
            sinf((i / (vert_num / 2.f)) * 3.14159265359f)) * 0.5f + vec2(-.35f, .45f),
            vec4(((i + (2 * vert_num / 3)) % vert_num) / (float)vert_num, ((i + (vert_num / 3)) % vert_num) / (float)vert_num, i / (float)vert_num, 1), vec2(1)));
    }
}

TestApp::~TestApp() {}

void TestApp::resize(int width, int height) {
    view.resize(width, height);
    frame_buffer.resize(width, height);
}

void TestApp::frame() {
    view.camera().update();

    frame_buffer.bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    batch.use_default_shader();
    batch.combined = glm::mat4(1);
    batch.begin(squares);
    batch.draw_rect({ vec2(-1), vec4(.4f, .4f, .4f, 1), vec2(0) }, { vec2(1), vec4(.4f, .4f, .4f, 1), vec2(0) });
    batch.end();

    batch.combined = view.camera().combined();
    batch.restart();
    batch.draw_rect(vec2(-0.2f), 0.2f, 0.3f, 3.14159265f * frame_count() / 200);
    batch.use_default_texture();
    batch.set_tint(.5f + .5f * std::sin(frame_count() / 120.f), 1, 1);
    batch.draw_tri_strip({ bsg2::Vertex(vec2(0), vec4(1), vec2(1)), bsg2::Vertex(vec2(0, 1), vec4(1), vec2(1)), bsg2::Vertex(vec2(1), vec4(1), vec2(1)) });
    batch.set_tint(vec4(1));
    batch.draw_tri_strip(strip_vertices);
    batch.draw_tri_fan(fan_vertices);
    batch.end();

    frame_buffer.unbind();

    view.apply();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    batch.set_shader(inversion);

    batch.combined = glm::mat4(1);
    batch.begin(frame_buffer);
    batch.draw_rect({ vec2(-1, -.5f), vec4(1), vec2(0) }, { vec2(0, .5f), vec4(1), vec2(1) });
    batch.end();

    batch.use_default_shader();
    batch.restart();
    batch.draw_rect({ vec2(0, -.5f), vec4(1), vec2(0) }, { vec2(1, .5f), vec4(1), vec2(1) });
    batch.end();
}