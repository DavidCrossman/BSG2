#pragma once

#include <GL/glew.h>

namespace bsg2 {
class Texture {
public:
    GLuint id;
    Texture(GLuint id);
    ~Texture();
    void set_mag_filter(GLuint mag_filter) const;
    void set_min_filter(GLuint min_filter) const;
    void set_horizontal_wrap(GLuint horizontal_wrap) const;
    void set_verical_wrap(GLuint vertical_wrap) const;
};
}