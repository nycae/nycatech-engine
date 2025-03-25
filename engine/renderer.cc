//
// Created by rplaz on 2025-01-30.
//

#include "renderer.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace nycatech {

Renderer::Renderer()
{
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

Renderer::~Renderer()
{
  SDL_DestroyWindow(window);
  SDL_Quit();
}


void Renderer::tick(World &world, Float32 time_delta)
{
  glLoadIdentity();
  glPushMatrix();
  glClearColor(1.0f, .5f, .5f, 1.0f);
  SmartPtr<Camera>    main_camera;
  SmartPtr<Transform> camera_transform;
  for (const auto &[camera, camera_transf] : world.entities_with<Camera, Transform>()) {
    if (!camera->is_main_camera) continue;
    main_camera = camera;
    camera_transform = camera_transf;
    break;
  }

  if (main_camera) {
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(camera_transform->data());
    glLoadMatrixf(glm::value_ptr(main_camera->projection_matrix()));
  }

  glMatrixMode(GL_MODELVIEW);
  for (const auto &[mesh, transform, shader, texture] :
       world.component_of_entity_or_nullptr<Mesh, Transform, ShaderProgram, Texture>()) {
    if (!mesh || !transform) continue;
    glPushMatrix();
    if (shader) {
      if (texture) {
        glBindTexture(GL_TEXTURE_2D, texture->id);
        glUniform1i(glGetUniformLocation(shader->id, "aTexture"), 0);
      }
      glUseProgram(shader->id);
      glUniformMatrix4fv(glGetUniformLocation(shader->id, "transform"), 1, GL_FALSE, transform->data());
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glPopMatrix();
  }
  glPopMatrix();
  SDL_GL_SwapWindow(window);
  glClear(GL_COLOR_BUFFER_BIT);
}

Mat4 Camera::projection_matrix() const
{
  return glm::perspective(glm::radians(fov), aspect_ratio, near_plane, far_plane);
}

}  // namespace nycatech