#include "registry.h"

void Registrable::Register(int id) {
  registry_id_ = id;
}

int Registrable::GetRegistryID() const {
  return registry_id_;
}