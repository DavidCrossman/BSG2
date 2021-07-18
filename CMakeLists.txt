# CMakeList.txt : CMake project for BSG2, include source and define
# project specific logic here.
#
#cmake_minimum_required (VERSION 3.8)

#project ("BSG2")

# Add source to this project's executable.
#add_executable (BSG2 "src/BSG2.cpp" "src/BSG2.h")

# TODO: Add tests and install targets if needed.
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
          "src/asset.cpp"
          "src/asset_manager.cpp"
          "src/batch.cpp"
          "src/camera.cpp"
          "src/shader.cpp"
          "src/sleep_util.cpp"
          "src/texture.cpp"
          "src/texture_manager.cpp"
          "src/main.cpp" "src/test_app.h" "src/test_app.cpp")
target_link_libraries(bsg2 PRIVATE GLEW::GLEW glfw glm soil2)

#add_custom_command(
#  TARGET bsg2
#  POST_BUILD
#  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/desktop/assets
#          ${CMAKE_BINARY_DIR}/assets)
