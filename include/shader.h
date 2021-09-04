#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "asset_manager.h"

namespace bsg2 {
class Shader {
public:
    GLuint program;
    inline Shader(GLuint program) : program(program) {}
};

class ShaderManager : public AssetManager<Shader> {
    void load(const std::string& path) override;
};
}