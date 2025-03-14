//
// Created by rplaz on 2025-01-30.
//

#ifndef NYCA_TECH_RENDERER_H
#define NYCA_TECH_RENDERER_H

#include <array>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "base.h"
#include "mesh.h"

namespace nycatech {

struct Color final : public Component {
  Color(Array<Float32, 3> color) : color(color) {};
  Array<Float32, 3> color;
};

struct Camera final : public Component {
  Array<Float32, 4> view_matrix;
  Array<Float32, 4> projection_matrix;
  float fov = 90.0f;
  float near_plane = 0.1f;
  float far_plane = 1000.0f;
  bool isMainCamera = false;
};

class Renderer {
 public:
  static Renderer& instance();

 private:
  Renderer();
  ~Renderer();

 public:
  void draw_frame();
  void render(const String& mesh_name, const SmartPtr<Transform>& transform);
  void buffer(const SmartPtr<Mesh>& mesh);

 public:
  SDL_Window* window;
  SDL_Surface* surface;
  SDL_GLContext context;
};

class RenderSystem final : public System {
 public:
  void tick(World& world, Float32 time_delta) override;
  void buffer(const SmartPtr<Mesh>& mesh);

 private:
  Renderer& renderer = Renderer::instance();
};

}  // namespace nycatech::render

#endif  // NYCA_TECH_RENDERER_H
