//
// Created by rplaz on 2025-01-30.
//

#include "renderer.h"

#include "gl/glew.h"

namespace nycatech::render {

Renderer::Renderer() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    return;
  }

  window = SDL_CreateWindow("nycatech", 20, 20, 1600, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (!window) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s", SDL_GetError());
    return;
  }

  context = SDL_GL_CreateContext(window);
  if (!context) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create gl context: %s", SDL_GetError());
    return;
  }

  glewInit();
}

void Renderer::DrawFrame() {
  SDL_GL_SwapWindow(window);
  glClear(GL_COLOR_BUFFER_BIT);
}

Renderer::~Renderer() {
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Renderer::Render(const SmartPtr<Mesh>& mesh, const SmartPtr<Transform>& transform, const SmartPtr<Color> color) {
  const auto sum = transform->rotation[0] + transform->rotation[1] + transform->rotation[2];
  glPushMatrix();
  glColor3f(color->color[0], color->color[1], color->color[2]);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glTranslatef(transform->position[0], transform->position[1], transform->position[2]);
  glRotatef(sum, transform->rotation[0] / sum, transform->rotation[1] / sum, transform->rotation[2] / sum);
  glScalef(transform->scale[0], transform->scale[1], transform->scale[2]);
  glBindVertexArray(mesh->vao);
  glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.Size());
  glPopMatrix();
}

void Renderer::Buffer(const SmartPtr<Mesh>& mesh) {
  glGenVertexArrays(1, &mesh->vao);
  glBindVertexArray(mesh->vao);
  glGenBuffers(1, &mesh->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
  glBufferData(GL_ARRAY_BUFFER, mesh->vertices.Size() * sizeof(Vertex), mesh->vertices.Data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, nullptr);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
}

Renderer& Renderer::Instance() {
  static Renderer renderer;
  return renderer;
}

void RenderSystem::Tick(World& world, Float32 timeDelta) {
  for (auto entity : world.EntitiesWithComponent<Mesh, Transform, Color>()) {
    auto [mesh, transform, color] = entity->Get<Mesh, Transform, Color>();
    renderer.Render(mesh, transform, color);
  }
  renderer.DrawFrame();
}

void RenderSystem::Buffer(const SmartPtr<Mesh>& mesh) { renderer.Buffer(mesh); }

}  // namespace nycatech::render