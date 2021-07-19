cmake_minimum_required(VERSION 3.10)

project(bsg2 VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(soil2 CONFIG REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(bsg2 "")
target_sources(
  bsg2
  PRIVATE "src/application.cpp"
          "src/asset_manager.cpp"
          "src/batch.cpp"
          "src/camera.cpp"
          "src/shader.cpp"
          "src/sleep_util.cpp"
          "src/texture.cpp"
          "src/texture_manager.cpp"
          "src/main.cpp"
          "src/test_app.cpp")
target_link_libraries(bsg2 PRIVATE GLEW::GLEW glfw glm soil2)
