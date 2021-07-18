#include "texture_manager.h"

#include <SOIL2/SOIL2.h>

void TextureManager::load(std::string path) {
    std::string qualified_path = get_asset_dir() + "textures/" + path;
    assets.emplace(path, new Texture(SOIL_load_OGL_texture(qualified_path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
        SOIL_FLAG_GL_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT)));
}