#include "asset_manager.h"

AssetManager& AssetManager::Get() {
  static AssetManager singleton;
  return singleton;
}

void AssetManager::RegisterAsset(const std::string& name, std::unique_ptr<AssetMeta> asset) {
  assets_.insert({name, std::move(asset)});
}