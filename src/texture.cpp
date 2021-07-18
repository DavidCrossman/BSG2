#include "texture.h"

Texture::Texture(GLuint id) : id(id) {
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::~Texture() { glDeleteTextures(1, &id); }
