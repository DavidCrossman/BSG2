#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "asset_manager.h"

namespace bsg2 {
class Shader {
public:
    GLuint program;
    Shader(GLuint program);
    ~Shader();
};

class ShaderManager : public AssetManager<Shader> {
public:
    ShaderManager(const std::string& asset_dir = "assets/shaders/");
    void load(std::string path) override;
};
}