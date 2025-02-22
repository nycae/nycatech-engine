//
// Created by rplaz on 2025-01-30.
//

#ifndef NYCA_TECH_ENGINE_H
#define NYCA_TECH_ENGINE_H

#include "library/vector.h"
#include "renderer/mesh.h"
#include "renderer/model.h"
#include "library/package.h"
#include "renderer/renderer.h"

namespace nycatech {

using render::Model;
using render::Mesh;
using render::Renderer;

class Application {
 public:
  Application();
  ~Application();

  void Run();

 private:
  Vector<Mesh *> meshes;
  Model greenSuzane;
  Model redSuzane;
  Model blueSuzane;
  Renderer context;
  SDL_Event event;
};

}  // namespace nycatech

#endif  // NYCA_TECH_ENGINE_H
