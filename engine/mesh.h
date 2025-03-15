//
// Created by rplaz on 2025-02-01.
//

#ifndef NYCA_TECH_MESH_H
#define NYCA_TECH_MESH_H

#include <array>

#include "transform.h"

namespace nycatech {

struct Vertex {
  Vec3 position;
  Vec3 normal;
  Vec2 tex_coord;

  Vertex(Vec3 pos = {}, Vec3 normal = {}, Vec2 tex_coord = {}) : position(pos), normal(normal), tex_coord(tex_coord) {}
};

struct MeshComponent : public Component {
  MeshComponent(const String& name) : name(name) {}
  String name;
};

struct Mesh {
  Vector<Vertex> vertices;
  Uint32 vao = 0;
  Uint32 vbo = 0;
};

class MeshFactory {
 public:
  static MeshFactory& instance();

 public:
  SmartPtr<Mesh> from_file(const String& name, const String& path);
  SmartPtr<Mesh> from_string(const String& name, const String& content);
  SmartPtr<Mesh> get(const String& name);

 public:
  Map<String, SmartPtr<Mesh>> meshes;
};

}  // namespace nycatech
#endif  // NYCA_TECH_MESH_H
