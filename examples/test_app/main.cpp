#include "test_app.h"

int main(int argc, const char** argv) {
    bsg2::initialise_asset_managers("examples/test_app/assets/");

    bsg2::ApplicationConfiguration config;

    config.name = "BSG2";
    config.width = 1024;
    config.height = 768;
    config.frame_time_ms = 5;

    TestApp test_app(config);
    test_app.run();

    return 0;
}