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
#include "shader.h"
#include "transform.h"

namespace nycatech {

struct Mesh {
  Uint32 Vao, Vbo, Ebo;
  Uint32 Texture;
  Int32  IndexCount;
};

struct Model {
  Vector<Mesh> Meshes;
  static Model FromString(const String& Content);
  static Model FromFile(const String& Path);
};

struct Camera {
  Mat4 ProjectionMatrix() const;
  Mat4 ViewMatrix() const;

  Camera() = default;

  Float32 Fov = 90.0f;
  Float32 AspectRatio = 1600.0f / 900.0f;
  Float32 NearPlane = 0.1f;
  Float32 FarPlane = 100.0f;

  Transform Transform{ Vec3{ 0, 0, 2 } };
};

class Renderer final {
public:
  Renderer();
  ~Renderer();

public:
  void Render(const Vector<Model>& Models);

public:
  SDL_Window*   Window;
  SDL_Surface*  Surface;
  SDL_GLContext Context;
  ShaderProgram ModelShader;
  Camera        MainCamera;
};

}  // namespace nycatech

#endif  // NYCA_TECH_RENDERER_H
