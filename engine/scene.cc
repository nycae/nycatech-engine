//
// Created by rplaz on 2025-02-24.
//

#include "scene.h"

#include "flatbuffers/flatbuffers.h"
#include "mesh.h"
#include "renderer.h"
#include "scene_generated.h"
#include "transform.h"

namespace nycatech {

void Scene::add_system(const SmartPtr<System>& system) { world.systems.push_back(system); }

void Scene::update(Float32 time_delta) { world.tick(time_delta); }

Entity& Scene::create_entity() { return world.create_entity(); }

void Scene::load() {}

void Scene::unload() {}

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

void SceneManager::add_scene(const String& scene_name, const SmartPtr<Scene>& scene) { scenes.insert(make_pair(scene_name, scene)); }

bool SceneFactory::serialize_scene(const SmartPtr<Scene>& scene, const String& file_path) {
  flatbuffers::FlatBufferBuilder builder(1 << 20);  // 1mb
  auto offset_scene_name = builder.CreateString(scene->name);

  Vector<Uint8> component_types;
  Vector<flatbuffers::Offset<schemas::Entity>> offset_entities;
  for (const auto& entity : scene->world.entities) {
    Vector<flatbuffers::Offset<void>> offset_components;
    for (const auto& [_, component] : entity.components) {
      if (auto transform = dynamic_cast<Transform*>(component.get())) {
        auto position = schemas::CreateVector3(builder, transform->position[0], transform->position[1], transform->position[2]);
        auto rotation = schemas::CreateVector3(builder, transform->rotation[0], transform->rotation[1], transform->rotation[2]);
        auto scale = schemas::CreateVector3(builder, transform->local_scale[0], transform->local_scale[1], transform->local_scale[2]);
        auto offset_transform = schemas::CreateTransform(builder, position, rotation, scale);
        offset_components.push_back(offset_transform.Union());
        component_types.push_back(schemas::Component::Component_Transform);
      } else if (auto mesh = dynamic_cast<MeshComponent*>(component.get())) {
        auto mesh_name = builder.CreateString(mesh->name);
        auto offset_mesh = schemas::CreateMesh(builder, mesh_name);
        offset_components.push_back(offset_mesh.Union());
        component_types.push_back(schemas::Component::Component_Mesh);
      }
    }
    auto serialized_types = builder.CreateVector(component_types);
    auto serialized_components = builder.CreateVector(offset_components);
    auto entity_offset = schemas::CreateEntity(builder, serialized_types, serialized_components);
    offset_entities.push_back(entity_offset);
  }

  Vector<Uint64> offset_systems;
  for (const auto& system : scene->world.systems) {
    if (auto transform = dynamic_cast<RenderSystem*>(system.get())) {
      auto offset_system = schemas::System::System_Renderer;
      offset_systems.push_back(offset_system);
    }
  }

  auto serialized_entities = builder.CreateVector(offset_entities);
  auto serialized_systems = builder.CreateVector(offset_systems);
  auto serialized_scene = schemas::CreateScene(builder, offset_scene_name, serialized_entities, serialized_systems);

  builder.Finish(serialized_scene);

  FileWritter file(file_path.c_str(), std::ios::binary);
  file.write(reinterpret_cast<const char*>(builder.GetBufferPointer()), builder.GetSize());
  return true;
}

SmartPtr<Scene> SceneFactory::deserialize_scene(const String& name, const String& file_path) {
  auto scene = create_scene(name);
  FileReader reader(file_path, std::ios::binary);
  reader.unsetf(std::ios::skipws);
  reader.seekg(0, std::ios::end);
  auto file_size = reader.tellg();
  reader.seekg(0, std::ios::beg);
  Vector<Uint8> buffer;  //(std::istream_iterator<Uint8>(reader), std::istream_iterator<Uint8>());
  buffer.reserve(file_size);
  buffer.insert(buffer.begin(), std::istream_iterator<Uint8>(reader), std::istream_iterator<Uint8>());
  auto scene_deserialized = flatbuffers::GetRoot<schemas::Scene>(buffer.data());

  scene->name = *scene_deserialized->name();
  for (const auto& entity_deserialized : *scene_deserialized->entities()) {
    auto& entity = scene->create_entity();
    auto components_types_deserialized = entity_deserialized->components_type();
    auto components_deserialized = entity_deserialized->components();
    assert(components_types_deserialized->size() == components_deserialized->size());
    for (Uint64 i = 0; i < components_types_deserialized->size(); i++) {
      auto component_deserialized = (*components_deserialized)[i];
      auto component_type_deserialized = (*components_types_deserialized)[i];
      switch (component_type_deserialized) {
        case schemas::Component::Component_Transform: {
          auto transform_deserialized = static_cast<const schemas::Transform*>(component_deserialized);
          auto position = transform_deserialized->position();
          auto rotation = transform_deserialized->rotation();
          auto scale = transform_deserialized->scale();
          entity.add_component<Transform>(Vec3{position->x(), position->y(), position->z()},
                                          Vec3{rotation->x(), rotation->y(), rotation->z()},
                                          Vec3{scale->x(), scale->y(), scale->z()});
          break;
        }
        case schemas::Component::Component_Mesh: {
          auto mesh_deserialized = static_cast<const schemas::Mesh*>(component_deserialized);
          auto mesh_name = mesh_deserialized->name()->c_str();
          entity.add_component<MeshComponent>(mesh_name);
        }
        default:
          break;
      }
    }
  }

  for (const auto& system_deserialized : *scene_deserialized->systems()) {
    switch (system_deserialized) {
      case schemas::System::System_Renderer:
        scene->add_system(make_shared<RenderSystem>());
    }
  }
  return scene;
}

}  // namespace nycatech