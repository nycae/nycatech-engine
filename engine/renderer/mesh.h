//
// Created by rplaz on 2025-02-01.
//

#ifndef NYCA_TECH_MESH_H
#define NYCA_TECH_MESH_H

#include <array>

#include "library/transform.h"
#include "library/vector.h"
#include "tiny_obj_loader.h"

namespace nycatech::render {

struct Vertex {
  std::array<float, 3> position;
  std::array<float, 3> normal;
  std::array<float, 2> textCord;
};

class Mesh {
 public:
  static Mesh* FromFile(const char* path);
  static Mesh* FromStream(const std::istream& stream);

 public:
  Vector<Vertex> vertices;
  Uint32 vao;
  Uint32 vbo;

 private:
  static Mesh* LoadData(const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes);

 private:
  Mesh() = default;
  ~Mesh() = default;

  Mesh(Mesh&&) = delete;
  Mesh(const Mesh&) = delete;

  Mesh& operator=(const Mesh&) = delete;
  Mesh&& operator=(Mesh&&) = delete;
};

}  // namespace nycatech::render
#endif  // NYCA_TECH_MESH_H
