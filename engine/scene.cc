//
// Created by rplaz on 2025-02-24.
//

#include "scene.h"

#include <flatbuffers/flatbuffers.h>

#include <filesystem>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>

#include "mesh.h"
#include "renderer.h"
#include "scene_generated.h"
#include "transform.h"

namespace nycatech {

void Scene::add_system(const SmartPtr<System>& system)
{
  world.systems.push_back(system);
}

void Scene::update(Float32 time_delta)
{
  world.tick(time_delta);
}

Entity& Scene::create_entity()
{
  return world.create_entity();
}

void Scene::load()
{
}

void Scene::unload()
{
}

void SceneManager::load_scene(const String& scene_name)
{
  if (current_scene) {
    current_scene->unload();
  }
  current_scene = scenes[scene_name];
  current_scene->load();
}

void SceneManager::update(Float32 time_delta)
{
  if (current_scene) {
    current_scene->update(time_delta);
  }
}

SceneManager& SceneManager::instance()
{
  static SceneManager instance;
  return instance;
}

void SceneManager::add_scene(const String& scene_name, const SmartPtr<Scene>& scene)
{
  scenes.insert(make_pair(scene_name, scene));
}

bool SceneFactory::serialize_scene(const SmartPtr<Scene>& scene, const String& file_path)
{
  flatbuffers::FlatBufferBuilder builder(1 << 20);  // 1mb
  auto                           offset_scene_name = builder.CreateString(scene->name);

  Vector<flatbuffers::Offset<schemas::Entity>> offset_entities;
  for (const auto& entity : scene->world.entities) {
    Vector<Uint8>                     component_types;
    Vector<flatbuffers::Offset<void>> offset_components;
    for (const auto& [_, component] : entity.components) {
      if (auto transform = dynamic_cast<Transform*>(component.get())) {
        Vec3 t_position, t_scale, t_skew;
        Quat t_rotation_tmp;
        Vec4 t_perspective;
        glm::decompose(transform->transform_matrix, t_scale, t_rotation_tmp, t_position, t_skew, t_perspective);
        Vec3       t_rotation = glm::degrees(glm::eulerAngles(glm::conjugate(t_rotation_tmp)));
        const auto position = schemas::CreateVector3(builder, t_position.x, t_position.y, t_position.z);
        const auto rotation = schemas::CreateVector3(builder, t_rotation.x, t_rotation.y, t_rotation.z);
        const auto scale = schemas::CreateVector3(builder, t_scale.x, t_scale.y, t_scale.z);
        const auto offset_transform = schemas::CreateTransform(builder, position, rotation, scale);
        offset_components.push_back(offset_transform.Union());
        component_types.push_back(schemas::Component::Component_Transform);
      }
      else if (auto mesh = dynamic_cast<Mesh*>(component.get())) {
        const auto mesh_name = builder.CreateString("");
        const auto offset_mesh = schemas::CreateMesh(builder, mesh_name);
        offset_components.push_back(offset_mesh.Union());
        component_types.push_back(schemas::Component::Component_Mesh);
      }
      else if (auto shader = dynamic_cast<Shader*>(component.get())) {
        const auto shader_name = builder.CreateString("");
        const auto offset_shader = schemas::CreateMesh(builder, shader_name);
        offset_components.push_back(offset_shader.Union());
        component_types.push_back(schemas::Component::Component_Shader);
      }
      else if (auto camera = dynamic_cast<Camera*>(component.get())) {
        const auto offset_camera = schemas::CreateCamera(
            builder, camera->fov, camera->aspect_ratio, camera->near_plane, camera->far_plane, camera->is_main_camera);
        offset_components.push_back(offset_camera.Union());
        component_types.push_back(schemas::Component::Component_Camera);
      }
    }
    auto serialized_types = builder.CreateVector(component_types.data(), component_types.size());
    auto serialized_components = builder.CreateVector(offset_components.data(), offset_components.size());
    auto entity_offset = schemas::CreateEntity(builder, serialized_types, serialized_components);
    offset_entities.push_back(entity_offset);
  }

  Vector<Uint64> offset_systems;
  for (const auto& system : scene->world.systems) {
    if (auto transform = dynamic_cast<Renderer*>(system.get())) {
      auto offset_system = schemas::System::System_Renderer;
      offset_systems.push_back(offset_system);
    }
  }

  auto serialized_entities = builder.CreateVector(offset_entities.data(), offset_entities.size());
  auto serialized_systems = builder.CreateVector(offset_systems.data(), offset_entities.size());
  auto serialized_scene = schemas::CreateScene(builder, offset_scene_name, serialized_entities, serialized_systems);

  builder.Finish(serialized_scene);

  FileWritter file(file_path.c_str(), std::ios::binary);
  file.write(reinterpret_cast<const char*>(builder.GetBufferPointer()), builder.GetSize());
  return true;
}

SmartPtr<Scene> SceneFactory::deserialize_scene(const String& name, const String& file_path)
{
  auto          scene = create_scene(name);
  FileReader    file(file_path.c_str(), std::ios::binary);
  Vector<Uint8> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  auto          scene_deserialized = flatbuffers::GetRoot<schemas::Scene>(buffer.data());

  scene->name = *scene_deserialized->name()->c_str();
  for (const auto& entity_deserialized : *scene_deserialized->entities()) {
    auto& entity = scene->create_entity();
    auto  components_types_deserialized = entity_deserialized->components_type();
    auto  components_deserialized = entity_deserialized->components();
    assert(components_types_deserialized->size() == components_deserialized->size());
    for (Uint64 i = 0; i < components_types_deserialized->size(); i++) {
      auto component_deserialized = (*components_deserialized)[i];
      auto component_type_deserialized = (*components_types_deserialized)[i];
      switch (component_type_deserialized) {
        case schemas::Component::Component_Transform: {
          const auto transform_deserialized = static_cast<const schemas::Transform*>(component_deserialized);
          const auto position = transform_deserialized->position();
          const auto rotation = transform_deserialized->rotation();
          const auto scale = transform_deserialized->scale();
          entity.add_component<Transform>(Vec3{ position->x(), position->y(), position->z() },
                                          Vec3{ rotation->x(), rotation->y(), rotation->z() },
                                          Vec3{ scale->x(), scale->y(), scale->z() });
          break;
        }
        case schemas::Component::Component_Mesh: {
          const auto mesh_deserialized = static_cast<const schemas::Mesh*>(component_deserialized);
          const auto mesh_name = mesh_deserialized->name()->c_str();
          break;
        }
        case schemas::Component::Component_Camera: {
          const auto camera_deserialized = static_cast<const schemas::Camera*>(component_deserialized);
          const auto fov = camera_deserialized->fov();
          const auto aspect_ratio = camera_deserialized->aspect_ratio();
          const auto near_plane = camera_deserialized->near_plane();
          const auto far_plane = camera_deserialized->far_plane();
          const auto is_main_camera = camera_deserialized->is_main_camera();
          entity.add_component<Camera>(fov, aspect_ratio, near_plane, far_plane, is_main_camera);
          break;
        }
        case schemas::Component::Component_Shader: {
          const auto shader_deserialized = static_cast<const schemas::Shader*>(component_deserialized);
          const auto shader_name = shader_deserialized->name()->c_str();
          break;
        }
        default:
          break;
      }
    }
  }

  for (const auto& system_deserialized : *scene_deserialized->systems()) {
    switch (system_deserialized) {
      case schemas::System::System_Renderer:
        scene->add_system(make_shared<Renderer>());
    }
  }
  return scene;
}

}  // namespace nycatech