#pragma once

#include "asset.h"
#include <memory>

class AssetMeta {
 public:
  virtual std::shared_ptr<Asset> Load() = 0;
  virtual ~AssetMeta() {

  }
};