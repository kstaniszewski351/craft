#pragma once

#include "item_stack.h"

struct Inventory {
  int active_slot = 0;
  std::array<std::optional<ItemStack>, 9> slots;
};