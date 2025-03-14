//
// Created by rplaz on 2025-02-24.
//

#include "scene.h"

#include "mesh.h"
#include "renderer.h"
#include "scene.pb.h"
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
  protos::v1::Scene serialized;

  serialized.set_name(scene->name);

  for (const auto& scene_system : scene->world.systems) {
    using protos::v1::System;
    auto system = serialized.add_systems();
    if (auto render_system = dynamic_pointer_cast<RenderSystem>(scene_system)) {
      system->set_type(System::Type::System_Type_Renderer);
    }
  }

  for (const auto& scene_entity : scene->world.entities) {
    auto entity = serialized.add_entities();
    for (const auto& [component_type, entity_component] : scene_entity.components) {
      auto component = entity->add_components();
      if (component_type == typeid(Transform)) {
        const auto transform = static_pointer_cast<Transform>(entity_component);
        component->mutable_transform()->mutable_position()->set_x(transform->position[0]);
        component->mutable_transform()->mutable_position()->set_y(transform->position[1]);
        component->mutable_transform()->mutable_position()->set_z(transform->position[2]);

        component->mutable_transform()->mutable_rotation()->set_x(transform->rotation[0]);
        component->mutable_transform()->mutable_rotation()->set_y(transform->rotation[1]);
        component->mutable_transform()->mutable_rotation()->set_z(transform->rotation[2]);

        component->mutable_transform()->mutable_scale()->set_x(transform->local_scale[0]);
        component->mutable_transform()->mutable_scale()->set_y(transform->local_scale[1]);
        component->mutable_transform()->mutable_scale()->set_z(transform->local_scale[2]);
      } else if (component_type == typeid(MeshComponent)) {
        const auto mesh = static_pointer_cast<MeshComponent>(entity_component);
        component->mutable_mesh()->set_name(mesh->name);
      }
    }
  }

  FileWritter file(file_path.c_str(), std::ios::binary);
  return serialized.SerializeToOstream(&file);
}

SmartPtr<Scene> SceneFactory::deserialize_scene(const String& name, const String& file_path) {
  protos::v1::Scene serialized_scene;
  FileReader reader(file_path, std::ios::binary);
  serialized_scene.ParseFromIstream(&reader);

  auto scene = create_scene(name);
  scene->name = serialized_scene.name();
  for (const auto& serialized_system : serialized_scene.systems()) {
    using protos::v1::System;
    switch (serialized_system.type()) {
      case System::Type::System_Type_Renderer:
        scene->add_system(make_shared<RenderSystem>());
      default:
        break;
    }
  }

  for (const auto& serialized_entity : serialized_scene.entities()) {
    auto& entity = scene->world.create_entity();
    for (const auto& serialized_component : serialized_entity.components()) {
      if (serialized_component.has_mesh()) {
        const auto& serialized_mesh = serialized_component.mesh();
        entity.add_component(make_shared<MeshComponent>(serialized_mesh.name()));
      } else if (serialized_component.has_transform()) {
        const auto& serialized_transform = serialized_component.transform();
        const auto transform = make_shared<Transform>(
            Vec3{serialized_transform.position().x(), serialized_transform.position().y(), serialized_transform.position().z()},
            Vec3{serialized_transform.rotation().x(), serialized_transform.rotation().y(), serialized_transform.rotation().z()},
            Vec3{serialized_transform.scale().x(), serialized_transform.scale().y(), serialized_transform.scale().z()});
        entity.add_component(transform);
      }
    }
  }

  return scene;
}

}  // namespace nycatech