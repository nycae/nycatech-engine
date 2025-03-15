//
// Created by rplaz on 2025-02-01.
//

#include "mesh.h"

namespace nycatech {

SmartPtr<Mesh> MeshFactory::from_file(const String& name, const String& path) {
  FileReader file(path);
  String file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  return from_string(name, file_content);
}

SmartPtr<Mesh> MeshFactoryfrom_string(const StringBuilder& stream) { return nullptr; }

MeshFactory& MeshFactory::instance() {
  static MeshFactory instance;
  return instance;
}

SmartPtr<Mesh> MeshFactory::get(const String& name) { return meshes[name]; }

SmartPtr<Mesh> MeshFactory::from_string(const String& name, const String& content) {
  auto mesh = make_shared<Mesh>();

  Vector<Vec3> temp_positions;
  Vector<Vec3> temp_normals;
  Vector<Vec2> temp_tex_coords;
  Vector<String> face_elems;

  String line;
  StringReader file(content);

  while (getline(file, line)) {
    String type;
    StringReader iss(line);
    iss >> type;

    if (type == "v") {
      Vec3 position;
      iss >> position[0] >> position[1] >> position[2];
      temp_positions.push_back(position);
    } else if (type == "vn") {
      Vec3 normal;
      iss >> normal[0] >> normal[1] >> normal[2];
      temp_normals.push_back(normal);
    } else if (type == "vt") {
      Vec2 tex_coord;
      iss >> tex_coord[0] >> tex_coord[1];
      temp_tex_coords.push_back(tex_coord);
    } else if (type == "f") {
      String face_data;
      while (iss >> face_data) {
        face_elems.push_back(face_data);
      }
    }
  }

  for (const auto& face : face_elems) {
    String vertex_data;
    StringReader face_stream(face);
    Array<int, 3> indices = {-1, -1, -1};

    for (int i = 0; i < 3; ++i) {
      getline(face_stream, vertex_data, '/');
      if (vertex_data.empty()) continue;
      indices[i] = stoi(vertex_data) - 1;
    }

    Vertex vertex;
    if (indices[0] >= 0 && indices[0] < temp_positions.size()) vertex.position = temp_positions[indices[0]];
    if (indices[1] >= 0 && indices[1] < temp_tex_coords.size()) vertex.tex_coord = temp_tex_coords[indices[1]];
    if (indices[2] >= 0 && indices[2] < temp_normals.size()) vertex.normal = temp_normals[indices[2]];

    mesh->vertices.push_back(vertex);
  }

  meshes.insert({name, mesh});
  return meshes[name];
}

}  // namespace nycatech