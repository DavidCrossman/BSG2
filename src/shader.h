#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "asset_manager.h"

class Shader {
public:
    GLuint program;
    inline Shader(GLuint program) : program(program) {}
};

class ShaderManager : public AssetManager<Shader> {
    void load(std::string path) override;
};
