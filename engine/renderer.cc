//
// Created by rplaz on 2025-01-30.
//

#include "renderer.h"

#include <glad/glad.h>
#include <tiny_gltf.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace nycatech {

Renderer::Renderer()
{
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    return;
  }

  Window = SDL_CreateWindow("nycatech", 1600, 900, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (!Window) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create Window: %s", SDL_GetError());
    return;
  }

  Context = SDL_GL_CreateContext(Window);
  if (!Context) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create gl Context: %s", SDL_GetError());
    return;
  }

  gladLoadGL();
}

Renderer::~Renderer()
{
  SDL_DestroyWindow(Window);
  SDL_Quit();
}

void Renderer::Render(const Vector<Model>& Models)
{
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(ModelShader.Id);
  glUniformMatrix4fv(glGetUniformLocation(ModelShader.Id, "CameraProjection"),
                     1,GL_FALSE,glm::value_ptr(MainCamera.ProjectionMatrix()));
  glUniformMatrix4fv(glGetUniformLocation(ModelShader.Id, "CameraTransform"), 1, GL_FALSE, glm::value_ptr(MainCamera.ViewMatrix()));
  for (const auto& Model : Models) {
    for (const auto& Mesh : Model.Meshes) {
      const auto Transform = glm::identity<Mat4>();
      glUniformMatrix4fv(glGetUniformLocation(ModelShader.Id, "Transform"), 1, GL_FALSE, glm::value_ptr(Transform));
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, Mesh.Texture);
      glUniform1i(glGetUniformLocation(ModelShader.Id, "Texture"), 0);
      glBindVertexArray(Mesh.Vao);
      glDrawElements(GL_TRIANGLES, Mesh.IndexCount, GL_UNSIGNED_SHORT, nullptr);
    }
  }
  SDL_GL_SwapWindow(Window);
}

void Renderer::AddLight(const Lights& LightSources)
{
  glUniform1i(glGetUniformLocation(ModelShader.Id, "LightCount"), LightSources.Colors.size());
  for (Int32 i = 0; i < LightSources.Colors.size(); i++) {
    const String Prefix = "Lights[" + std::to_string(i) + "]";
    const auto& Position = LightSources.Positions[i];
    const auto& Color = LightSources.Colors[i];
    const auto& Intensity = LightSources.Intensities[i];
    const auto& Range = LightSources.Ranges[i];
    const auto& Type = LightSources.Types[i];
    glUniform3fv(glGetUniformLocation(ModelShader.Id, (Prefix + ".Position").c_str()),1, glm::value_ptr(Position));
    glUniform3fv(glGetUniformLocation(ModelShader.Id, (Prefix + ".Color").c_str()),1, glm::value_ptr(Color));
    glUniform1f(glGetUniformLocation(ModelShader.Id, (Prefix + ".Intensity").c_str()),Intensity);
    glUniform1f(glGetUniformLocation(ModelShader.Id, (Prefix + ".Range").c_str()),Range);
    glUniform1i(glGetUniformLocation(ModelShader.Id, (Prefix + ".Type").c_str()), Type);
  }
}

Mat4 Camera::ProjectionMatrix() const
{
  return glm::perspective(glm::radians(Fov), AspectRatio, NearPlane, FarPlane);
}

Mat4 Camera::ViewMatrix() const
{
  return Self.Transform.ViewMatrix();
}

void Model::FromFile(const String& Path, Model& Model)
{
  FileReader File(Path, std::ios::binary);
  String     FileContent((std::istreambuf_iterator<char>(File)), std::istreambuf_iterator<char>());
  return FromString(FileContent, Model);
}

void Model::FromString(const String& Content, Model& Model)
{
  tinygltf::Model    GlbModel;
  tinygltf::TinyGLTF Loader;
  String             Err, Warn;

  assert(Loader.LoadBinaryFromMemory(&GlbModel, &Err, &Warn, (const Byte*)Content.data(), Content.size(), ""));
  assert(Warn.empty());
  assert(Err.empty());

  for (const auto& GlbMesh : GlbModel.meshes) {
    for (const auto& Primitive : GlbMesh.primitives) {
      Model.Meshes.push_back(Mesh{});
      Mesh& Mesh = Model.Meshes.back();

      const auto& PosAccessor = GlbModel.accessors[Primitive.attributes.at("POSITION")];
      const auto& PosView = GlbModel.bufferViews[PosAccessor.bufferView];
      const auto& PosBuffer = GlbModel.buffers[PosView.buffer];
      const auto& Positions = (const Float32*)&PosBuffer.data[PosView.byteOffset + PosAccessor.byteOffset];

      const auto& UvAccessor = GlbModel.accessors[Primitive.attributes.at("TEXCOORD_0")];
      const auto& UvView = GlbModel.bufferViews[UvAccessor.bufferView];
      const auto& UvBuffer = GlbModel.buffers[UvView.buffer];
      const auto& Uvs = (const Float32*)&UvBuffer.data[UvView.byteOffset + UvAccessor.byteOffset];

      const auto& NormalsAccessor = GlbModel.accessors[Primitive.attributes.at("NORMAL")];
      const auto& NormalsView = GlbModel.bufferViews[NormalsAccessor.bufferView];
      const auto& NormalsBuffer = GlbModel.buffers[UvView.buffer];
      const auto& Normals = (const Float32*)&NormalsBuffer.data[NormalsView.byteOffset + NormalsAccessor.byteOffset];

      const auto& IndexAccessor = GlbModel.accessors[Primitive.indices];
      const auto& IndexView = GlbModel.bufferViews[IndexAccessor.bufferView];
      const auto& IndexBuffer = GlbModel.buffers[IndexView.buffer];
      const auto& Indices = (const Uint16*)&IndexBuffer.data[IndexView.byteOffset + IndexAccessor.byteOffset];

      const Uint32 BufferSize = PosView.byteLength + NormalsView.byteLength + UvView.byteLength;

      glGenVertexArrays(1, &Mesh.Vao);
      glBindVertexArray(Mesh.Vao);

      glGenBuffers(1, &Mesh.Vbo);
      glBindBuffer(GL_ARRAY_BUFFER, Mesh.Vbo);

      glBufferData(GL_ARRAY_BUFFER, BufferSize, nullptr, GL_STATIC_DRAW);
      glBufferSubData(GL_ARRAY_BUFFER, 0, PosView.byteLength, Positions);
      glBufferSubData(GL_ARRAY_BUFFER, PosView.byteLength, PosView.byteLength, Normals);
      glBufferSubData(GL_ARRAY_BUFFER, PosView.byteLength + NormalsView.byteLength, UvView.byteLength, Uvs);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)PosView.byteLength);
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(PosView.byteLength + NormalsView.byteLength));

      glGenBuffers(1, &Mesh.Ebo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mesh.Ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexView.byteLength, Indices, GL_STATIC_DRAW);
      Mesh.IndexCount = IndexAccessor.count;

      if (Primitive.material < 0) continue;
      const auto& Material = GlbModel.materials[Primitive.material];
      if (Material.pbrMetallicRoughness.baseColorTexture.index < 0) continue;
      const auto& Texture = GlbModel.textures[Material.pbrMetallicRoughness.baseColorTexture.index];
      const auto& Image = GlbModel.images[Texture.source];

      glGenTextures(1, &Mesh.Texture);
      glBindTexture(GL_TEXTURE_2D, Mesh.Texture);
      glTexImage2D(
          GL_TEXTURE_2D, 0, GL_RGBA, Image.width, Image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image.image.data());
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
  }
}

void Mesh::Unload()
{
  const auto buffers = Array<Uint32, 3>{ Vao, Vbo, Ebo };
  glDeleteBuffers(3, buffers.data());
}

void Lights::Add(const Lights::Light& Light)
{
  Types.push_back(Light.Type);
  Positions.push_back(Light.Position);
  Colors.push_back(Light.Color);
  Intensities.push_back(Light.Intensity);
  Ranges.push_back(Light.Range);
}

}  // namespace nycatech