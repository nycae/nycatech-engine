//
// Created by rplaz on 2025-02-01.
//

#include "mesh.h"

#include <glad/glad.h>

#include "cgltf.h"

namespace nycatech {

SmartPtr<Mesh> Mesh::from_file(const String& path)
{
  FileReader file(path, std::ios::binary);
  String     file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  return from_string(file_content);
}

SmartPtr<Mesh> Mesh::from_string(const String& content)
{
  auto          mesh = make_shared<Mesh>();
  cgltf_options options = {};
  cgltf_data*   data = nullptr;
  cgltf_result  result = cgltf_parse(&options, content.data(), content.size(), &data);

  if (result != cgltf_result_success) return nullptr;

  result = cgltf_load_buffers(&options, data, nullptr);
  if (result != cgltf_result_success) {
    cgltf_free(data);
    return nullptr;
  }

  cgltf_mesh*      gltf_mesh = &data->meshes[0];
  cgltf_primitive* prim = &gltf_mesh->primitives[0];

  cgltf_accessor* pos_accessor = nullptr;
  cgltf_accessor* normal_accessor = nullptr;
  cgltf_accessor* uv_accessor = nullptr;

  for (size_t i = 0; i < prim->attributes_count; ++i) {
    cgltf_attribute* attr = &prim->attributes[i];
    switch (attr->type) {
      case cgltf_attribute_type_position:
        pos_accessor = attr->data;
        break;
      case cgltf_attribute_type_normal:
        normal_accessor = attr->data;
        break;
      case cgltf_attribute_type_texcoord:
        uv_accessor = attr->data;
        break;
      default:
        break;
    }
  }

  if (!pos_accessor || !normal_accessor || !uv_accessor) {
    cgltf_free(data);
    return nullptr;
  }

  mesh->vertices.resize(pos_accessor->count);
  for (size_t i = 0; i < pos_accessor->count; ++i) {
    auto pos
        = (float*)((char*)pos_accessor->buffer_view->buffer->data + pos_accessor->offset + pos_accessor->stride * i);
    mesh->vertices[i].position = glm::vec3(pos[0], pos[1], pos[2]);
    auto normal = (float*)((char*)normal_accessor->buffer_view->buffer->data + normal_accessor->offset
                           + normal_accessor->stride * i);
    mesh->vertices[i].normal = glm::vec3(normal[0], normal[1], normal[2]);
    auto uv = (float*)((char*)uv_accessor->buffer_view->buffer->data + uv_accessor->offset + uv_accessor->stride * i);
    mesh->vertices[i].tex_coord = glm::vec2(uv[0], uv[1]);
  }

  if (prim->indices) {
    auto indices = (unsigned short*)((char*)prim->indices->buffer_view->buffer->data + prim->indices->offset);
    mesh->indices.assign(indices, indices + prim->indices->count);
  }

  glGenVertexArrays(1, &mesh->vao);
  glGenBuffers(1, &mesh->vbo);
  glGenBuffers(1, &mesh->ebo);

  glBindVertexArray(mesh->vao);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
  glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), mesh->vertices.data(), GL_STATIC_DRAW);

  if (!mesh->indices.empty()) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(Uint32), mesh->indices.data(), GL_STATIC_DRAW);
  }

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));

  glBindVertexArray(0);
  cgltf_free(data);
  return mesh;
}

bool Vertex::operator==(const Vertex& other) const
{
  return Self.position == other.position && Self.normal == other.normal && Self.tex_coord == other.tex_coord;
}

}  // namespace nycatech