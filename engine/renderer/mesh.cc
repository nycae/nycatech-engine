//
// Created by rplaz on 2025-02-01.
//

#include "mesh.h"

namespace nycatech::render {

SmartPtr<Mesh> Mesh::from_file(const char* path) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn;
  std::string err;

  tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path);
  if (!err.empty()) {
    fprintf(stderr, "unable to load model: %s", err.c_str());
    return nullptr;
  }

  return load_data(attrib, shapes);
}

SmartPtr<Mesh> Mesh::from_string(const StringBuilder& stream) { return nullptr; }

SmartPtr<Mesh> Mesh::load_data(const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes) {
  auto mesh = std::make_shared<Mesh>();
  for (const auto& shape : shapes) {
    for (const auto& index : shape.mesh.indices) {
      mesh->vertices.push_back(Vertex{
          .position = {attrib.vertices[index.vertex_index * 3],
                       attrib.vertices[index.vertex_index * 3 + 1],
                       attrib.vertices[index.vertex_index * 3 + 2]},
          .normal = attrib.normals.empty() ? std::array<float, 3>{0.0f, 0.0f, 0.0f}
                                           : std::array<float, 3>{attrib.normals[index.normal_index * 3],
                                                                  attrib.normals[index.normal_index * 3 + 1],
                                                                  attrib.normals[index.normal_index * 3 + 2]},
          .textCord = attrib.texcoords.empty() ? std::array<float, 2>{0.0f, 0.0f}
                                               : std::array<float, 2>{attrib.texcoords[index.texcoord_index * 2],
                                                                      attrib.texcoords[index.texcoord_index * 2 + 1]},
      });
    }
  }

  return mesh;
}

}  // namespace nycatech::render