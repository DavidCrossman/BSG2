#pragma once

#include <GL/glew.h>
#include "asset_manager.h"

namespace bsg2 {
class Texture {
public:
    GLuint id;
    Texture(GLuint id);
    ~Texture();
    void set_mag_filter(GLuint mag_filter) const;
    void set_min_filter(GLuint min_filter) const;
    void set_horizontal_wrap(GLuint horizontal_wrap) const;
    void set_vertical_wrap(GLuint vertical_wrap) const;
};

class TextureManager : public AssetManager<const Texture> {
public:
    TextureManager(const std::string& asset_dir = "assets/textures/");
    void load(std::string path) override;
};
}