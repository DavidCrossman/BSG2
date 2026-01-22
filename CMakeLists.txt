cmake_minimum_required(VERSION 3.21)

project(bsg2 VERSION 1.0)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GLEW CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(soil2 CONFIG REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(BSG2_SOURCES 
            "src/application.cpp"
            "src/batch.cpp"
            "src/camera.cpp"
            "src/file_util.cpp"
            "src/frame_buffer.cpp"
            "src/shader.cpp"
            "src/sleep_util.cpp"
            "src/texture.cpp"
            "src/vertex.cpp"
            "src/viewport.cpp")

set(BSG2_HEADERS 
            "include/application.h"
            "include/asset_manager.h"
            "include/batch.h"
            "include/camera.h"
            "include/file_util.h"
            "include/frame_buffer.h"
            "include/shader.h"
            "include/sleep_util.h"
            "include/texture.h"
            "include/vertex.h"
            "include/viewport.h")

if (WIN32)
    add_library(bsg2 STATIC ${BSG2_SOURCES})
elseif (UNIX)
    add_library(bsg2 SHARED ${BSG2_SOURCES})
endif ()

target_link_libraries(bsg2 PRIVATE GLEW::GLEW glfw soil2)

set(INSTALL_DIR ${PROJECT_SOURCE_DIR}/out/bsg2)
install(TARGETS bsg2 CONFIGURATIONS ${CMAKE_BUILD_TYPE} DESTINATION ${INSTALL_DIR}/lib/${CMAKE_BUILD_TYPE})
install(FILES ${BSG2_HEADERS} DESTINATION ${INSTALL_DIR}/include/bsg2)

add_executable(bsg2_example ${BSG2_SOURCES} "examples/test_app/main.cpp" "examples/test_app/test_app.cpp")
target_link_libraries(bsg2_example PRIVATE GLEW::GLEW glfw soil2)
