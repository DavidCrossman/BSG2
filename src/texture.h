#pragma once

#include <GL/glew.h>

#include "asset.h"

class Texture : public Asset {
   public:
    GLuint id;
    Texture(GLuint id);
    ~Texture();
};