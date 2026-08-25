#pragma once

#include "atlas.h"
#include "block.h"

Atlas* GenerateIconAtlas(std::vector<const Block*>& blocks, int icon_size, const Atlas& blocks_atlas);