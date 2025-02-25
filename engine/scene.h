//
// Created by rplaz on 2025-02-24.
//

#ifndef NYCA_TECH_SCENE_H
#define NYCA_TECH_SCENE_H

#include "ecs.h"

namespace nycatech {

class Scene {
 public:
  friend class SceneFactory;

  Scene(const String& name) : name(name) {};
  ~Scene() = default;

  Entity& create_entity();

  void add_system(const SmartPtr<System>& system);
  void update(Float32 time_delta);
  void load();
  void unload();

 public:
  World world;
  String name;
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

class SceneFactory {
 public:
  static SmartPtr<Scene> create_scene(const String& name) { return make_shared<Scene>(name); }
  static bool serialize_scene(const SmartPtr<Scene>& scene, const String& file_path);
  static SmartPtr<Scene> deserialize_scene(const String& name, const String& file_path);
};

}  // namespace nycatech

#endif  // NYCA_TECH_SCENE_H
