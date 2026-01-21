#include "texture.h"

#include <SOIL2/SOIL2.h>

namespace bsg2 {
Texture::Texture(GLuint id) : id(id) {}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

GLint Texture::get_width() const {
    GLint width;
    with_binding([&width] {
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    });
    return width;
}

GLint Texture::get_height() const {
    GLint height;
    with_binding([&height] {
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    });
    return height;
}

void Texture::set_mag_filter(GLuint mag_filter) {
    with_binding([mag_filter] {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
    });
}

void Texture::set_min_filter(GLuint min_filter) {
    with_binding([min_filter] {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    });
}

void Texture::set_horizontal_wrap(GLuint horizontal_wrap) {
    with_binding([horizontal_wrap] {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, horizontal_wrap);
    });
}

void Texture::set_vertical_wrap(GLuint vertical_wrap) {
    with_binding([vertical_wrap] {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vertical_wrap);
    });
}

void Texture::with_binding(std::function<void()> f) const {
    GLint prev_id;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_id);
    if (id != prev_id) glBindTexture(GL_TEXTURE_2D, id);

    f();

    if (id != prev_id) glBindTexture(GL_TEXTURE_2D, prev_id);
}

TextureManager::TextureManager(const std::string& asset_dir) : AssetManager(asset_dir) {}

void TextureManager::load(std::string path) {
    if (assets.find(path) != assets.end()) return;
    std::string qualified_path = asset_dir + path;
    assets.emplace(path, new Texture(SOIL_load_OGL_texture(qualified_path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
        SOIL_FLAG_GL_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT)));
}
}