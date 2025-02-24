//
// Created by rplaz on 2025-01-30.
//

#ifndef NYCA_TECH_ENGINE_H
#define NYCA_TECH_ENGINE_H

#include "library/ecs.h"
#include "library/package.h"
#include "renderer/mesh.h"
#include "renderer/renderer.h"

namespace nycatech {

class Application {
 public:
  Application();
  ~Application();

  void run();

 private:
  World world;
  SDL_Event event;
};

}  // namespace nycatech

#endif  // NYCA_TECH_ENGINE_H
