//
// Created by rplaz on 2025-01-30.
//

#ifndef NYCA_TECH_ENGINE_H
#define NYCA_TECH_ENGINE_H

#include "ecs.h"
#include "scene.h"
#include "mesh.h"
#include "package.h"
#include "renderer.h"

namespace nycatech {

class Application {
 public:
  Application();
  ~Application();

  void run();

 private:
  SmartPtr<Scene> scene;
  ThreadPool tp;
  SDL_Event event;
};

}  // namespace nycatech

#endif  // NYCA_TECH_ENGINE_H
