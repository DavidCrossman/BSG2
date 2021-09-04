#pragma once
#include <map>
#include <string>

namespace bsg2 {
void initialise_asset_managers(const std::string& asset_dir = "assets/");
const std::string& get_asset_dir();

template <class T>
class AssetManager {
protected:
    std::map<std::string, const T*> assets;

public:
    AssetManager();
    ~AssetManager();
    virtual void load(const std::string& path) = 0;
    const T* get(const std::string& path) const;
    const T* load_get(const std::string& path);
};

template <class T>
inline AssetManager<T>::AssetManager() {}

template <class T>
inline AssetManager<T>::~AssetManager() {
    for (const auto& kv : assets) {
        delete kv.second;
    }
}

template <class T>
inline const T* AssetManager<T>::get(const std::string& path) const {
    return assets.find(path)->second;
}

template <class T>
inline const T* AssetManager<T>::load_get(const std::string& path) {
    if (assets.find(path) == assets.end()) load(path);
    return assets.find(path)->second;
}
}