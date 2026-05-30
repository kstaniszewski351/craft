#pragma once

#include <stdexcept>
#include <string>
#include <memory>
#include <unordered_map>
#include "asset.h"
#include "asset_meta.h"


class AssetManager {

 public:
  
  static AssetManager& Get();

  void RegisterAsset(const std::string& name, std::unique_ptr<AssetMeta> asset);

  template<typename T>
  std::shared_ptr<T> GetAsset(const std::string& name) {
    auto cache_find = cache_.find(name);

    if(cache_find != cache_.end()) {
      if(cache_find->second.expired()) {
        return CreateAsset<T>(name);
      }

      std::shared_ptr<T> asset = std::dynamic_pointer_cast<T>(cache_find->second.lock());
      if(!asset) {
        throw std::runtime_error("Asset in cache is the wrong type\n");
      }

      return asset;
    }
    return CreateAsset<T>(name);

  };
 private:
  std::unordered_map<std::string, std::unique_ptr<AssetMeta>> assets_;
  std::unordered_map<std::string, std::weak_ptr<Asset>> cache_;
  
  template<typename T>
  std::shared_ptr<T> CreateAsset(const std::string& name) {
    auto find_meta = assets_.find(name);
    if(find_meta == assets_.end()) {
      throw std::runtime_error("Asset not found\n");
    }

    AssetMeta* asset_meta = find_meta->second.get();

    std::shared_ptr<T> asset = std::dynamic_pointer_cast<T>(asset_meta->Load());

    if(!asset) {
      throw std::runtime_error("Asset is the wrong type\n");
    }

    std::weak_ptr<Asset> weak_ptr = std::weak_ptr<Asset>(std::static_pointer_cast<Asset>(asset));

    cache_.insert({name, weak_ptr});

    return asset;
  };

 private:
;
};