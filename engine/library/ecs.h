//
// Created by rplaz on 2025-02-23.
//

#ifndef NYCA_TECH_ECS_H
#define NYCA_TECH_ECS_H

#include <unordered_map>

#include "base.h"
#include "hash_map.h"
#include "hash_set.h"
#include "vector.h"

namespace nycatech {

class Component;
class Entity;
class World;
class System;

// typedef std::function<void(World& world, float timeDelta)> System;

class Entity {
 public:
  template <typename T>
  void AddComponent(const SmartPtr<T>& component) {
    components.insert({typeid(T), component});
  }

  template <typename T>
  void RemoveComponents() {
    components.erase(typeid(T));
  }

  template <typename T>
  bool HasComponents() {
    return components.contains(typeid(T));
  }

  template <typename T, typename T1, typename... T2>
  bool HasComponents() {
    return components.contains(typeid(T)) && HasComponents<T1, T2...>();
  }

  template <typename... Ts>
  auto Get() {
    return std::make_tuple(static_pointer_cast<Ts>(components.at(typeid(Ts)))...);
  }

 private:
  std::unordered_map<Type, SmartPtr<Component>> components;
};

class World {
 public:
  Entity& CreateEntity();
  void Tick(Float32 timeDelta);

 public:
  template <typename H, typename... T>
  Vector<Entity*> EntitiesWithComponent() {
    Vector<Entity*> result;
    for (auto& entity : entities) {
      if (!entity.HasComponents<H, T...>()) continue;
      result.PushBack(&entity);
    }
    return result;
  };

 public:
  Vector<Entity> entities;
  Vector<SmartPtr<System>> systems;
};

class Component {
 public:
  virtual ~Component() = default;
};

class System {
 public:
  virtual void Tick(World& world, Float32 timeDelta) = 0;
};

}  // namespace nycatech

#endif  // NYCA_TECH_ECS_H
