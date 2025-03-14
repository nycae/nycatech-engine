//
// Created by rplaz on 2025-02-01.
//

#include "mesh.h"

namespace nycatech {

SmartPtr<Mesh> MeshFactory::from_file(const String& name, const String& path) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn;
  std::string err;

  tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());
  if (!err.empty()) {
    fprintf(stderr, "unable to load model: %s", err.c_str());
    return nullptr;
  }

  meshes.insert({name, load_data(attrib, shapes)});
  return meshes[name];
}

SmartPtr<Mesh> MeshFactoryfrom_string(const StringBuilder& stream) { return nullptr; }

SmartPtr<Mesh> MeshFactory::load_data(const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes) {
  auto mesh = std::make_shared<Mesh>();
  for (const auto& shape : shapes) {
    for (const auto& index : shape.mesh.indices) {
      mesh->vertices.push_back(Vertex{
          Vec3{attrib.vertices[index.vertex_index * 3], attrib.vertices[index.vertex_index * 3 + 1], attrib.vertices[index.vertex_index * 3 + 2]},
          attrib.normals.empty()
              ? Vec3{0.0f, 0.0f, 0.0f}
              : Vec3{attrib.normals[index.normal_index * 3], attrib.normals[index.normal_index * 3 + 1], attrib.normals[index.normal_index * 3 + 2]},
          attrib.texcoords.empty() ? Vec2{0.0f, 0.0f}
                                   : Vec2{attrib.texcoords[index.texcoord_index * 2], attrib.texcoords[index.texcoord_index * 2 + 1]},
      });
    }
  }

  return mesh;
}

MeshFactory& MeshFactory::intance() {
  static MeshFactory instance;
  return instance;
}

SmartPtr<Mesh> MeshFactory::get(const String& name) { return meshes[name]; }

}  // namespace nycatech