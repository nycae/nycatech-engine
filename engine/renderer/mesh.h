//
// Created by rplaz on 2025-02-01.
//

#ifndef NYCA_TECH_MESH_H
#define NYCA_TECH_MESH_H

#include <array>

#include "library/transform.h"
#include "tiny_obj_loader.h"

namespace nycatech::render {

struct Vertex {
  Array<Float32, 3> position;
  Array<Float32, 3> normal;
  Array<Float32, 2> textCord;
};

class Mesh : public Component {
 public:
  static SmartPtr<Mesh> from_file(const char* path);
  static SmartPtr<Mesh> from_string(const StringBuilder& stream);

 public:
  Vector<Vertex> vertices;
  Uint32 vao;
  Uint32 vbo;

 private:
  static SmartPtr<Mesh> load_data(const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes);

 public:
  Mesh() = default;
  ~Mesh() = default;

  Mesh(Mesh&&) = delete;
  Mesh(const Mesh&) = delete;

  Mesh& operator=(const Mesh&) = delete;
  Mesh&& operator=(Mesh&&) = delete;
};

}  // namespace nycatech::render
#endif  // NYCA_TECH_MESH_H
