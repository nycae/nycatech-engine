//
// Created by rplaz on 2025-02-24.
//

#include "scene.h"

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

}  // namespace nycatech