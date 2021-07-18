#pragma once

#include <GL/glew.h>

namespace bsg2 {
class Texture {
public:
    GLuint id;
    Texture(GLuint id);
    ~Texture();
};
}