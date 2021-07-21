#pragma once

#include <GL/glew.h>

namespace bsg2 {
class Texture {
public:
    GLuint id;
    Texture(GLuint id);
    ~Texture();
    void set_mag_filter(GLuint mag_filter);
    void set_min_filter(GLuint min_filter);
    void set_horizontal_wrap(GLuint horizontal_wrap);
    void set_verical_wrap(GLuint vertical_wrap);
};
}