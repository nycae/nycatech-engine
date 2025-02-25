//
// Created by rplaz on 2025-02-24.
//

#include "scene.h"

#include "flatbuffers/flatbuffers.h"
#include "mesh.h"
#include "scene_generated.h"
#include "transform.h"

namespace nycatech {

void Scene::add_system(const SmartPtr<System>& system) { world.systems.push_back(system); }

void Scene::update(Float32 time_delta) { world.tick(time_delta); }

Entity& Scene::create_entity() { return world.CreateEntity(); }

void SceneManager::load_scene(const String& scene_name) {
  if (current_scene) {
    current_scene->unload();
  }
  current_scene = scenes[scene_name];
  current_scene->load();
}

void SceneManager::update(Float32 time_delta) {
  if (current_scene) {
    current_scene->update(time_delta);
  }
}

SceneManager& SceneManager::instance() {
  static SceneManager instance;
  return instance;
}

void SceneManager::add_scene(const String& scene_name, const SmartPtr<Scene>& scene) {
  scenes.insert(make_pair(scene_name, scene));
}

bool SceneFactory::serialize_scene(const SmartPtr<Scene>& scene, const String& file_path) {
  flatbuffers::FlatBufferBuilder builder(1 << 20);  // 1mb
  auto offset_scene_name = builder.CreateString(scene->name);

  Vector<flatbuffers::Offset<schemas::Entity>> offset_entities;
  for (const auto& entity : scene->world.entities) {
    Vector<flatbuffers::Offset<schemas::Component>> offset_components;
    for (const auto& [_, component] : entity.components) {
      if (auto transform = dynamic_cast<Transform*>(component.get())) {
      } else if (auto mesh = dynamic_cast<Mesh*>(component.get())) {
      }
    }
    auto serialized_components = builder.CreateVector(offset_components);
    auto entity_offset = schemas::CreateEntity(builder, serialized_components);
    offset_entities.push_back(entity_offset);
  }

  Vector<flatbuffers::Offset<schemas::System>> offset_systems;
  for (const auto& system : scene->world.systems) {
  }

  auto serialized_entities = builder.CreateVector(offset_entities);
  auto serialized_systems = builder.CreateVector(offset_systems);

  auto serialized_scene = schemas::CreateScene(builder, offset_scene_name, serialized_entities);
  return true;
}

}  // namespace nycatech