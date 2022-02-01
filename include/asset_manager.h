#pragma once

#include <unordered_map>
#include <string>
#include "file_util.h"

namespace bsg2 {
template <class T>
class AssetManager {
public:
    AssetManager(const std::string& asset_dir = "assets/") {
        set_asset_dir(asset_dir);
    }
    AssetManager(const AssetManager& other) = delete;
    ~AssetManager() {
        for (const auto& kv : assets) {
            delete kv.second;
        }
    }
    const std::string& asset_dir() const {
        return m_asset_dir;
    }
    void set_asset_dir(const std::string& asset_dir) {
        m_asset_dir = find_full_dir(asset_dir);
    }
    virtual void load(std::string path) = 0;
    T& get(const std::string& path) {
        return *assets.find(path)->second;
    }
    T& load_get(const std::string& path) {
        if (assets.find(path) == assets.end()) load(path);
        return *assets.find(path)->second;
    }
protected:
    std::unordered_map<std::string, T*> assets;
    std::string m_asset_dir;
};
}