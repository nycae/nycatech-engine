//
// Created by rplaz on 2025-01-30.
//

#ifndef NYCA_TECH_ENGINE_H
#define NYCA_TECH_ENGINE_H

#include "ecs.h"
#include "mesh.h"
#include "package.h"
#include "renderer.h"
#include "scene.h"

namespace nycatech {

class Application {
public:
  Application(SmartPtr<Scene> scene = SceneFactory::create_scene("main"));
  ~Application();

  void run();

private:
  SmartPtr<Scene> scene;
  ThreadPool      tp;
  SDL_Event       event;
};

}  // namespace nycatech

#endif  // NYCA_TECH_ENGINE_H
