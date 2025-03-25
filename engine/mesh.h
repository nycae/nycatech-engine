//
// Created by rplaz on 2025-02-01.
//

#ifndef NYCA_TECH_MESH_H
#define NYCA_TECH_MESH_H

#include <array>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <utility>

#include "transform.h"

namespace nycatech {

struct Vertex {
  Vec3 position;
  Vec3 normal;
  Vec2 tex_coord;

  bool operator==(const Vertex& other) const;

  Vertex(Vec3 position = {}, Vec3 normal = {}, Vec2 tex_coord = {})
      : position(position), normal(normal), tex_coord(tex_coord)
  {
  }
};
}  // namespace nycatech

template <>
struct ::std::hash<nycatech::Vertex> {
  size_t operator()(const nycatech::Vertex& v) const
  {
    using namespace nycatech;
    return ((hash<Vec3>()(v.position) ^ (hash<Vec3>()(v.normal) << 1)) >> 1) ^ (hash<Vec2>()(v.tex_coord) << 1);
  }
};

namespace nycatech {

struct Mesh : public Component {
  Vector<Vertex> vertices;
  Vector<Uint32> indices;
  Uint32         vao = 0;
  Uint32         vbo = 0;
  Uint32         ebo = 0;

  static SmartPtr<Mesh> from_file(const String& file);
  static SmartPtr<Mesh> from_string(const String& content);
};

}  // namespace nycatech
#endif  // NYCA_TECH_MESH_H
