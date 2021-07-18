#pragma once
#include <map>
#include <string>

#include "asset.h"

void initialise_asset_managers();
const std::string &get_asset_dir();

template <class T>
class AssetManager {
   protected:
    std::map<std::string, T *> assets;

   public:
    AssetManager();
    ~AssetManager();
    virtual void load(std::string path) = 0;
    T *get(std::string path);
    T *load_get(std::string path);
};

template <class T>
inline AssetManager<T>::AssetManager() {}

template <class T>
inline AssetManager<T>::~AssetManager() {
    for (const auto &kv : assets) {
        delete kv.second;
    }
}

template <class T>
inline T *AssetManager<T>::get(std::string path) {
    return assets.find(path)->second;
}

template <class T>
inline T *AssetManager<T>::load_get(std::string path) {
    if (assets.find(path) == assets.end()) load(path);
    return assets.find(path)->second;
}
