#pragma once

#include <stdexcept>
#include <unordered_map>
#include <memory>


class Registrable {
 public:
  int GetRegistryID() const;
  void Register(int id);
  virtual ~Registrable() = default;
 protected:
  int registry_id_;
};

template<typename T>
class Registry {
 public:
  const T& Register(std::string name, std::unique_ptr<T> object) {
    int id = next_id;
    if(name_to_id_.contains(name)) {
      throw std::runtime_error("object already registered");
    }
    object->Register(id);
    objects_[id] = std::move(object);
    name_to_id_[name] = id;
    next_id ++;
    return Get(id);
  };
  const T& Get(int id) const {
    return *objects_.at(id).get();
  };
  int GetID(std::string name) const {
    return name_to_id_.at(name);
  };
  const T& Get(std::string name) const {
    return Get(GetID(name));
  };
  const std::unordered_map<int, std::unique_ptr<T>>& GetObjects() {
    return objects_;
  };
 private:
  std::unordered_map<int, std::unique_ptr<T>> objects_;
  std::unordered_map<std::string, int> name_to_id_;
  int next_id = 1;
};
