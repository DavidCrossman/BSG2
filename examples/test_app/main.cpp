#include <iostream>

#include "test_app.h"

#include <filesystem>

int main(int argc, const char** argv) {
    bsg2::initialise_asset_managers("examples/test_app/assets/");

    bsg2::WindowConfiguration config;

    config.name = "BSG2";
    config.width = 1024;
    config.height = 768;
    config.frame_time_ms = 5;

    bsg2::execute([](GLFWwindow* window) { return new TestApp(window, 1024, 768); }, config);

    return 0;
}