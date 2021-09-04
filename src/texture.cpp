#include "texture.h"

namespace bsg2 {
Texture::Texture(GLuint id) : id(id) {}

Texture::~Texture() { glDeleteTextures(1, &id); }

void Texture::set_mag_filter(GLuint mag_filter) const {
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::set_min_filter(GLuint min_filter) const {
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::set_horizontal_wrap(GLuint horizontal_wrap) const {
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, horizontal_wrap);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::set_verical_wrap(GLuint vertical_wrap) const {
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vertical_wrap);
    glBindTexture(GL_TEXTURE_2D, 0);
}

}