//
// Created by rplaz on 2025-01-30.
//

#ifndef NYCA_TECH_ENGINE_H
#define NYCA_TECH_ENGINE_H

#include "renderer.h"

namespace nycatech {

class Application {
public:
  Application();
  ~Application();

  void run();

private:
  ThreadPool tp;
  SDL_Event  event;
  Model      Model;
  Renderer   Renderer;
};

}  // namespace nycatech

#endif  // NYCA_TECH_ENGINE_H
