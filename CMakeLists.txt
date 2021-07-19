cmake_minimum_required(VERSION 3.10)

project(bsg2 VERSION 1.0)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(soil2 CONFIG REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(BSG2_SOURCES 
            "src/application.cpp"
            "src/asset_manager.cpp"
            "src/batch.cpp"
            "src/camera.cpp"
            "src/shader.cpp"
            "src/sleep_util.cpp"
            "src/texture.cpp"
            "src/texture_manager.cpp")

set(BSG2_HEADERS 
            "include/application.h"
            "include/asset_manager.h"
            "include/batch.h"
            "include/camera.h"
            "include/shader.h"
            "include/sleep_util.h"
            "include/texture.h"
            "include/texture_manager.h"
            "include/vertex.h")

add_library(bsg2 STATIC ${BSG2_SOURCES})
target_link_libraries(bsg2 PRIVATE GLEW::GLEW glfw glm soil2)

set(CUSTOM_INSTALL_DIR ${PROJECT_SOURCE_DIR}/../../Libraries/bsg2)
install(TARGETS bsg2 ARCHIVE DESTINATION ${CUSTOM_INSTALL_DIR}/lib)
install(FILES ${BSG2_HEADERS} DESTINATION ${CUSTOM_INSTALL_DIR}/include/bsg2)

add_executable(bsg2_example ${BSG2_SOURCES} "examples/main.cpp" "examples/test_app.cpp")
target_link_libraries(bsg2_example PRIVATE GLEW::GLEW glfw glm soil2)
