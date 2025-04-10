//
// Created by rplaz on 2025-01-30.
//

#pragma once

#define SDL_MAIN_NOIMPL
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>

#include "base.h"
#include "shader.h"
#include "transform.h"

namespace nycatech {

struct Lights {
  enum Type : int {
    Directional = 0,
    Point = 1,
    Spot = 2
  };

  struct Light {
    Vec3    Position;
    Vec3    Color;
    Float32 Intensity;
    Float32 Range;
    Type    Type;
  };

  void Add(const Light& Light);

  Vector<Vec3>    Positions;
  Vector<Vec3>    Colors;
  Vector<Float32> Intensities;
  Vector<Float32> Ranges;
  Vector<Type>    Types;
};

struct Mesh {
  Uint32 Vao, Vbo, Ebo;
  Uint32 Texture;
  Int32  IndexCount;

  void Unload();
};

struct Model {
  Vector<Mesh> Meshes;

  static void FromString(const String& Content, Model& Model);
  static void FromFile(const String& Path, Model& Model);
};

struct Camera {
  Mat4 ProjectionMatrix() const;
  Mat4 ViewMatrix() const;

  Camera() = default;

  Float32 Fov = 90.0f;
  Float32 AspectRatio = 1600.0f / 900.0f;
  Float32 NearPlane = 0.1f;
  Float32 FarPlane = 100.0f;

  Transform Transform{ Vec3{ 2, 2, 2 } };
};

class Renderer final {
public:
  Renderer();
  ~Renderer();

public:
  void AddLight(const Lights& LightSources);
  void Render(const Vector<Model>& Models);

public:
  SDL_Window*   Window;
  SDL_Surface*  Surface;
  SDL_GLContext Context;
  ShaderProgram ModelShader;
  Camera        MainCamera;
};

}  // namespace nycatech
