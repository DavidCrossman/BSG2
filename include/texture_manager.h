#pragma once

#include "asset_manager.h"
#include "texture.h"

namespace bsg2 {
class TextureManager : public AssetManager<Texture> {
    void load(const std::string& path) override;
};
}