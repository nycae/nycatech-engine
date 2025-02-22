//
// Created by rplaz on 2025-01-30.
//

#ifndef NYCA_TECH_RENDERER_H
#define NYCA_TECH_RENDERER_H

#include <array>

#include "SDL.h"
#include "model.h"
#include "mesh.h"

namespace nycatech::render {

class Renderer {
 public:
  Renderer();
  ~Renderer();

 public:
  void DrawFrame();
  void Render(const Model &model);
  void Buffer(Mesh *mesh);

 public:
  SDL_Window* window;
  SDL_Surface* surface;
  SDL_GLContext context;
};

}  // namespace nycatech::render

#endif  // NYCA_TECH_RENDERER_H
