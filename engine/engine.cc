//
// Created by rplaz on 2025-01-30.
//

#include "engine.h"

namespace nycatech {

void Application::Run() {
  while (true) {
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) break;
    context.Render(redSuzane);
    context.Render(blueSuzane);
    context.Render(greenSuzane);
    redSuzane.Translate({(redSuzane.transform.position[0] < 1.5f) ? 0.004f : -3.0f, 0, 0});
    blueSuzane.Translate({0, (blueSuzane.transform.position[1] < 1.5f) ? 0.004f : -3.0f, 0});
    greenSuzane.Translate({0, 0, (greenSuzane.transform.position[2] < 1.5f) ? 0.004f : -3.0f});
    redSuzane.Rotate({1, 0, 0});
    blueSuzane.Rotate({0, 1, 0});
    greenSuzane.Rotate({0, 0, 1});
    context.DrawFrame();
  }
}

Application::Application()  {
  auto suzane = render::Mesh::FromFile("../../../assets/ogre.obj");
  if (!suzane) {
    fprintf(stderr, "unable to load model");
  }
  context.Buffer(suzane);
  redSuzane.mesh = suzane;
  blueSuzane.mesh = suzane;
  greenSuzane.mesh = suzane;
  redSuzane.transform = {.position = {0, 0, 0}, .rotation = {0, 0, 0}, .scale = {0.1, 0.1, 0.1}};
  blueSuzane.transform = {.position = {0, 0, 0}, .rotation = {0, 0, 0}, .scale = {0.1, 0.1, 0.1}};
  greenSuzane.transform = {.position = {0, 0, 0}, .rotation = {0, 0, 0}, .scale = {0.1, 0.1, 0.1}};
  redSuzane.color = {1, .5, .5};
  greenSuzane.color = {.5, 1, .5};
  blueSuzane.color = {.5, .5, 1};
}

Application::~Application() { delete[] meshes.Data(); }

}  // namespace nycatech