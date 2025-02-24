//
// Created by rplaz on 2025-02-24.
//

#ifndef NYCA_TECH_SCENE_H
#define NYCA_TECH_SCENE_H

#include "ecs.h"

namespace nycatech {

class Scene {
 public:
  Scene() = default;
  ~Scene() = default;

  Entity& create_entity();

  void add_system(const SmartPtr<System>& system);
  void update(Float32 time_delta);
  void load();
  void unload();

 private:
  World world;
};

class SceneManager {
 public:
  static SceneManager& instance();

  ~SceneManager() = default;

 private:
  SceneManager() = default;

  void load_scene(const String& scene_name);
  void add_scene(const String& scene_name, const SmartPtr<Scene>& scene);
  void update(Float32 time_delta);

 private:
  Map<String, SmartPtr<Scene>> scenes;
  SmartPtr<Scene> current_scene;
};

}  // namespace nycatech

#endif  // NYCA_TECH_SCENE_H
