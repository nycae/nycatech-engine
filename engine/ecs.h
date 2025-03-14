//
// Created by rplaz on 2025-02-23.
//

#ifndef NYCA_TECH_ECS_H
#define NYCA_TECH_ECS_H

#include "base.h"

namespace nycatech {

class Component;
class Entity;
class World;
class System;

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
  Entity& create_entity();
  void tick(Float32 time_delta);

 public:
  template <typename T>
  auto components_of_type() {
    Vector<SmartPtr<T>> result;
    for (auto& entity : entities) {
      for (auto& [type, component] : entity.components) {
        if (typeid(T) != type) continue;
        result.push_back(dynamic_pointer_cast<T>(component));
      }
    }
    return result;
  };

  template <typename T, typename... M>
  auto entities_with() {
    Vector<Tuple<SmartPtr<T>, SmartPtr<M>...>> result;
    for (auto& entity : entities) {
      if (!entity.has_components<T, M...>()) continue;
      result.push_back(entity.get<T, M...>());
    }
    return result;
  }

 public:
  Vector<Entity> entities;
  Vector<SmartPtr<System>> systems;
};

struct Component {
  virtual ~Component() = default;
};

class System {
 public:
  virtual void tick(World& world, Float32 time_delta) = 0;
};

}  // namespace nycatech

#endif  // NYCA_TECH_ECS_H
