//
// Created by rplaz on 2025-02-23.
//

#ifndef NYCA_TECH_ECS_H
#define NYCA_TECH_ECS_H

#include <bitset>

#include "base.h"

namespace nycatech {

class Component;
class Entity;
class World;
class System;

constexpr Uint64 MaxComponents = 64;
using ComponentType = std::bitset<MaxComponents>;

class Entity {
 public:
  Entity() = default;
  Entity(Entity&&) = default;

 public:
  template <typename T, typename... Args>
  void add_component(Args&&... args) {
    components.insert({get_component_type_id<T>(), make_shared<T>(std::forward<Args>(args)...)});
    component_signature.set(get_component_type_id<T>());
  }

  template <typename T>
  void remove_component() {
    const auto id = get_component_type_id<T>();
    components.erase(id);
    component_signature.reset(id);
  }

  template <typename T>
  SmartPtr<T> get_component() {
    return static_pointer_cast<T>(components[get_component_type_id<T>()]);
  }

  template <typename... T>
  Tuple<SmartPtr<T>...> get_components() {
    return make_tuple(get_component<T>()...);
  }

  template <typename T>
  bool has_component() const {
    return components.contains(get_component_type_id<T>());
  }

  template <typename... T>
  bool has_components() const {
    return (has_components<T>() && ...);
  }

 public:
  template <typename T>
  static Uint64 get_component_type_id() {
    static Uint64 type_id = next_component_type_id++;
    return type_id;
  }

  inline static Uint64 next_component_type_id = 0;

 public:
  Map<Type, SmartPtr<Component>> components;
  ComponentType component_signature;
};

class World {
 public:
  Entity& create_entity();
  void tick(Float32 time_delta);

 public:
  template <typename... M>
  auto entities_with() {
    Vector<Tuple<SmartPtr<M>...>> result;
    ComponentType signature = get_component_signature<M...>();

    for (auto& entity : entities) {
      if ((entity.component_signature & signature) != signature)
        continue;
      result.push_back(entity.get_components<M...>());
    }
    return result;
  }

  template <typename... M>
  auto component_of_entity_or_nullptr() {
    Vector<Tuple<SmartPtr<M>...>> result;
    for (auto& entity : entities) {
      result.push_back(entity.get_components<M...>());
    }
    return result;
  }

 private:
  template <typename... T>
  ComponentType get_component_signature() {
    ComponentType signature;
    (signature.set(Entity::get_component_type_id<T>()), ...);
    return signature;
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
