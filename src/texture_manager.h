#pragma once

#include "asset_manager.h"
#include "texture.h"

class TextureManager : public AssetManager<Texture> {
    void load(std::string path) override;
};