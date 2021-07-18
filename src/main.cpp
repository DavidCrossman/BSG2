#include <iostream>

#include "test_app.h"

int main(int argc, const char** argv) {
    TestApp app(1024, 768);
    bsg2::WindowConfiguration config;

    config.name = "BSG2";
    config.width = 1024;
    config.height = 768;
    config.frame_time_ms = 5;

    bsg2::execute(&app, config);

    return 0;
}