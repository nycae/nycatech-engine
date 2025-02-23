//
// Created by rplaz on 2025-01-30.
//

#ifndef NYCA_TECH_RENDERER_H
#define NYCA_TECH_RENDERER_H

#include <array>

#include "SDL.h"
#include "mesh.h"

namespace nycatech::render {

class Color final : public Component {
 public:
  Color(Array<Float32, 3> color) : color(color) {};
  Array<Float32, 3> color;
};

class Renderer {
 public:
  static Renderer& Instance();

 private:
  Renderer();
  ~Renderer();

 public:
  void DrawFrame();
  void Render(const SmartPtr<Mesh>& mesh, const SmartPtr<Transform>& transform, const SmartPtr<Color> color);
  void Buffer(const SmartPtr<Mesh>& mesh);

 public:
  SDL_Window* window;
  SDL_Surface* surface;
  SDL_GLContext context;
};

class RenderSystem final : public System {
 public:
  void Tick(World& world, Float32 timeDelta) override;
  void Buffer(const SmartPtr<Mesh>& mesh);

 private:
  Renderer& renderer = Renderer::Instance();
};

}  // namespace nycatech::render

#endif  // NYCA_TECH_RENDERER_H
