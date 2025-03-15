//
// Created by rplaz on 2025-01-30.
//

#include "renderer.h"

#include <glad/glad.h>

namespace nycatech {

Renderer::Renderer() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    return;
  }

  window = SDL_CreateWindow("nycatech", 1600, 900, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (!window) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
    return;
  }

  context = SDL_GL_CreateContext(window);
  if (!context) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create gl context: %s", SDL_GetError());
    return;
  }

  gladLoadGL();
}

void Renderer::draw_frame() {
  SDL_GL_SwapWindow(window);
  glClear(GL_COLOR_BUFFER_BIT);
}

Renderer::~Renderer() {
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Renderer::render(const SmartPtr<MeshComponent>& mesh_comp, const SmartPtr<Transform>& transform) {
  if (!mesh_comp) {
    return;
  }

  const auto mesh = MeshFactory::instance().get(mesh_comp->name);
  glPushMatrix();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (transform) {
    auto matrix = transform->transform_matrix();
    glLoadMatrixf(matrix.inner.data());
  }
  glBindVertexArray(mesh->vao);
  glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());
  glPopMatrix();
}

bool Renderer::buffer(SmartPtr<Mesh> mesh) {
  glGenVertexArrays(1, &mesh->vao);
  glBindVertexArray(mesh->vao);
  glGenBuffers(1, &mesh->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
  glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), mesh->vertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, nullptr);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
  return true;
}

Renderer& Renderer::instance() {
  static Renderer renderer;
  return renderer;
}
bool Renderer::buffer(SmartPtr<ShaderProgram> program) {
  program->id = glCreateProgram();
  for (const auto& shader : program->shaders) {
    glAttachShader(program->id, shader->id);
  }
  glLinkProgram(program->id);
  GLint success;
  glGetProgramiv(program->id, GL_LINK_STATUS, &success);
  return success;
}

bool Renderer::compile(SmartPtr<Shader> shader) {
  const char* source = shader->source.c_str();
  shader->id = glCreateShader(shader->type);
  glShaderSource(shader->id, 1, &source, nullptr);
  glCompileShader(shader->id);
  GLint success;
  glGetShaderiv(shader->id, GL_COMPILE_STATUS, &success);
  return success;
}

void RenderSystem::tick(World& world, Float32 time_delta) {
  glLoadIdentity();
  glPushMatrix();

  SmartPtr<Camera> main_camera;
  SmartPtr<Transform> camera_transform;
  for (const auto& [camera, camera_transf] : world.entities_with<Camera, Transform>()) {
    if (!camera->is_main_camera) {
      continue;
    }
    main_camera = camera;
    camera_transform = camera_transf;
    break;
  }

  if (main_camera) {
    auto camera_matrix = camera_transform->transform_matrix();
    auto projection_matrix = main_camera->projection_matrix();

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(&projection_matrix.inner[0]);
    glLoadMatrixf(&camera_matrix.inner[0]);
  }
  glMatrixMode(GL_MODELVIEW);
  for (const auto& [mesh, transform, shader] :
       world.component_of_entity_or_nullptr<MeshComponent, Transform, ShaderComponent>()) {
    if (!mesh) {
      continue;
    }
    if (shader) {
      const auto shader_program = ShaderFactory::instance().get(shader->name);
      glUseProgram(shader_program->id);
    }
    renderer.render(mesh, transform);
  }
  glPopMatrix();
  renderer.draw_frame();
}

RenderSystem::RenderSystem() {
  for (const auto& [_, mesh] : MeshFactory::instance().meshes) {
    renderer.buffer(mesh);
  }

  for (const auto& [_, shader] : ShaderFactory::instance().shaders) {
    renderer.compile(shader);
  }

  for (const auto& [_, shader_program] : ShaderFactory::instance().shader_programs) {
    renderer.buffer(shader_program);
  }
}

TransformMatrix Camera::projection_matrix() const {
  const float tan_half_fov = std::tan(0.5f * fov * 3.14159265358979323846f / 180.0f);
  const float range = near_plane - far_plane;

  TransformMatrix projection_matrix;
  projection_matrix.at(0, 0) = 1.0f / (aspect_ratio * tan_half_fov);
  projection_matrix.at(1, 1) = 1.0f / tan_half_fov;
  projection_matrix.at(2, 2) = (-near_plane - far_plane) / range;
  projection_matrix.at(2, 3) = 2.0f * far_plane * near_plane / range;
  projection_matrix.at(3, 2) = 1.0f;

  return projection_matrix;
}

}  // namespace nycatech