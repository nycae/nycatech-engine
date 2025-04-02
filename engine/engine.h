//
// Created by rplaz on 2025-01-30.
//

#pragma once

#include "renderer.h"
#include "resource_manager.h"

namespace nycatech {

class Application {
public:
  Application();
  ~Application() = default;

  void Run();

private:
  ThreadPool      Tp;
  SDL_Event       event;
  Renderer        Renderer;
  Lights          LightSources;
  ResourceManager ResourceManager;
};

}  // namespace nycatech