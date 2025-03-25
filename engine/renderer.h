//
// Created by rplaz on 2025-01-30.
//

#ifndef NYCA_TECH_RENDERER_H
#define NYCA_TECH_RENDERER_H

#include <array>

#define SDL_MAIN_NOIMPL
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>

#include "base.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

namespace nycatech {

struct Camera final : public Component {
  Mat4 projection_matrix() const;

  Camera(float fov = 90.0f,
         float aspect_ratio = 1600.0f / 900.0f,
         float near_plane = 0.1f,
         float far_plane = 1000.0f,
         bool  is_main_camera = false)
      : fov(fov),
        aspect_ratio(aspect_ratio),
        near_plane(near_plane),
        far_plane(far_plane),
        is_main_camera(is_main_camera)
  {
  }

  float fov = 90.0f;
  float aspect_ratio = 1600.0f / 900.0f;
  float near_plane = 0.1f;
  float far_plane = 1000.0f;
  bool  is_main_camera = false;
};

class Renderer final : public System {
public:
  Renderer();
  ~Renderer();

public:
  virtual void tick(World &world, Float32 time_delta) override;

public:
  SDL_Window   *window;
  SDL_Surface  *surface;
  SDL_GLContext context;
};

}  // namespace nycatech

#endif  // NYCA_TECH_RENDERER_H
