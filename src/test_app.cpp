#include "test_app.h"

using namespace glm;

mat3 TestApp::screen_to_world() {
    return mat3(2.0 / width, 0, 0, 0, -2.0 / height, 0, -1, 1, 1);
}

void TestApp::framebuffer_size_callback(int width, int height) {
    this->width = width;
    this->height = height;
    cam.aspect_ratio = width / (float)height;
}

TestApp::TestApp(int width, int height) : width(width), height(height) {
    cam.aspect_ratio = width / (float)height;
}

TestApp::~TestApp() {
    delete batch;
}

void TestApp::initialise() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    batch = new bsg2::Batch(shaders.load_get("base"));
    
    cam.update();

    squares = textures.load_get("squares.png");
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