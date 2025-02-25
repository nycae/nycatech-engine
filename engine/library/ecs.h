//
// Created by rplaz on 2025-02-23.
//

#ifndef NYCA_TECH_ECS_H
#define NYCA_TECH_ECS_H

#include <unordered_map>

#include "base.h"

namespace nycatech {

class Component;
class Entity;
class World;
class System;

// typedef std::function<void(World& world, float timeDelta)> System;

class Entity {
 public:
  Entity() = default;
  Entity(Entity&&) = default;

 public:
  template <typename T>
  void add_component(const SmartPtr<T>& component) {
    components.insert({typeid(T), component});
  }

  template <typename T>
  void remove_component() {
    components.erase(typeid(T));
  }

  template <typename T>
  bool has_components() {
    return components.contains(typeid(T));
  }

  template <typename T, typename T1, typename... T2>
  bool has_components() {
    return components.contains(typeid(T)) && has_components<T1, T2...>();
  }

  template <typename... Ts>
  auto get() {
    return make_tuple(static_pointer_cast<Ts>(components.at(typeid(Ts)))...);
  }

 public:
  Map<Type, SmartPtr<Component>> components;
};

class World {
 public:
  Entity& CreateEntity();
  void tick(Float32 time_delta);

 public:
  template <typename H, typename... T>
  Vector<Entity*> entities_with() {
    Vector<Entity*> result;
    for (auto& entity : entities) {
      if (!entity.has_components<H, T...>()) continue;
      result.push_back(&entity);
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
  virtual void tick(World& world, Float32 time_delta) = 0;
};

}  // namespace nycatech

#endif  // NYCA_TECH_ECS_H
