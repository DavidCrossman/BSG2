#pragma once

#include <functional>
#include <GL/glew.h>
#include "asset_manager.h"

namespace bsg2 {
class Texture {
public:
    GLuint id;
    Texture(GLuint id);
    Texture(const Texture& other) = delete;
    ~Texture();
    GLint get_width() const;
    GLint get_height() const;
    void set_mag_filter(GLuint mag_filter);
    void set_min_filter(GLuint min_filter);
    void set_horizontal_wrap(GLuint horizontal_wrap);
    void set_vertical_wrap(GLuint vertical_wrap);
private:
    void with_binding(std::function<void()> f) const;
};

class TextureManager : public AssetManager<Texture> {
public:
    TextureManager(const std::string& asset_dir = "assets/textures/");
    void load(std::string path) override;
};
}