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

  window = SDL_CreateWindow("nycatech", 1600, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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

void Renderer::render(const SmartPtr<MeshComponent>& mesh_comp, const SmartPtr<Transform>& transform, const SmartPtr<Color>& color) {
  const auto sum = transform->rotation[0] + transform->rotation[1] + transform->rotation[2];
  const auto mesh = MeshFactory::instance().get(mesh_comp->name);
  glPushMatrix();
  if (color) glColor3f(color->color[0], color->color[1], color->color[2]);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (transform) {
    glTranslatef(transform->position[0], transform->position[1], transform->position[2]);
    glRotatef(sum, transform->rotation[0] / sum, transform->rotation[1] / sum, transform->rotation[2] / sum);
    glScalef(transform->local_scale[0], transform->local_scale[1], transform->local_scale[2]);
  }
  glBindVertexArray(mesh->vao);
  glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());
  glPopMatrix();
}

void Renderer::buffer(const SmartPtr<Mesh>& mesh) {
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
}

Renderer& Renderer::instance() {
  static Renderer renderer;
  return renderer;
}

void RenderSystem::tick(World& world, Float32 time_delta) {
  for (const auto& [mesh, transform, color] : world.entities_with<MeshComponent, Transform, Color>()) {
    renderer.render(mesh, transform, color);
  }
  renderer.draw_frame();
}

void RenderSystem::buffer(const SmartPtr<Mesh>& mesh) {
  renderer.buffer(mesh);
}

RenderSystem::RenderSystem() {
  for (const auto& [_, mesh] : MeshFactory::instance().meshes) {
    buffer(mesh);
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