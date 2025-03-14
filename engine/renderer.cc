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

void Renderer::render(const String& mesh_name, const SmartPtr<Transform>& transform) {
  const auto sum = transform->rotation[0] + transform->rotation[1] + transform->rotation[2];
  const auto mesh = MeshFactory::intance().get(mesh_name);
  glPushMatrix();
  //  glColor3f(color->color[0], color->color[1], color->color[2]);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glTranslatef(transform->position[0], transform->position[1], transform->position[2]);
  glRotatef(sum, transform->rotation[0] / sum, transform->rotation[1] / sum, transform->rotation[2] / sum);
  glScalef(transform->local_scale[0], transform->local_scale[1], transform->local_scale[2]);
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
  for (const auto& [mesh, transform] : world.entities_with<MeshComponent, Transform>()) {
    renderer.render(mesh->name, transform);
  }
  renderer.draw_frame();
}

void RenderSystem::buffer(const SmartPtr<Mesh>& mesh) { renderer.buffer(mesh); }

}  // namespace nycatech::render