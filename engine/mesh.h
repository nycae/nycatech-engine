//
// Created by rplaz on 2025-02-01.
//

#ifndef NYCA_TECH_MESH_H
#define NYCA_TECH_MESH_H

#include <array>

#include "tiny_obj_loader.h"
#include "transform.h"

namespace nycatech {

struct Vertex {
  Vec3 position;
  Vec3 normal;
  Vec2 textCord;

  Vertex(Vec3 pos = {}, Vec3 normal = {}, Vec2 textCord = {}) : position(pos), normal(normal), textCord(textCord) {}
};

struct MeshComponent : public Component {
  MeshComponent(const String& name) : name(name) {}
  String name;
};

struct Mesh : public Component {
  Vector<Vertex> vertices;
  Uint32 vao = 0;
  Uint32 vbo = 0;
};

class MeshFactory {
 public:
  static MeshFactory& intance();

 public:
  SmartPtr<Mesh> from_file(const String& name, const String& path);
  SmartPtr<Mesh> get(const String& name);

 private:
  static SmartPtr<Mesh> load_data(const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes);

 private:
  Map<String, SmartPtr<Mesh>> meshes;
};

}  // namespace nycatech
#endif  // NYCA_TECH_MESH_H
