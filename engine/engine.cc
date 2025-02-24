//
// Created by rplaz on 2025-01-30.
//

#include "engine.h"

#include "renderer/renderer.h"

namespace nycatech {

using namespace render;

class RotatingSystem final : public System {
 public:
  void tick(World& world, Float32 time_delta) override {
    for (auto entity : world.entities_with<Transform>()) {
      auto [transform] = entity->get<Transform>();
      transform->rotate({0.f, 1.f, 0.f});
    }
  };
};

class ChangeColorSystem final : public System {
 public:
  Uint32 red_amount = 1;

  void tick(World& world, Float32 time_delta) override {
    red_amount += 1;
    if (red_amount >= 256) red_amount = 0;
    for (auto entity : world.entities_with<Color>()) {
      auto [color] = entity->get<Color>();
      Float32 newColor = red_amount / 256.f;
      color->color[1] = newColor;
    }
  }
};

void Application::run() {
  while (true) {
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) break;
    world.tick(0.f);
  }
}

Application::Application() : world() {
  auto ogre = render::Mesh::from_file("../../../assets/robot.obj");
  if (!ogre) {
    fprintf(stderr, "unable to load model");
  }

  auto& ogre_blue = world.CreateEntity();
  ogre_blue.add_component(ogre);
  ogre_blue.add_component(Transform::Make({-0.f, -0.6f, 0.f}, {0.f, 0.f, 0.f}, {0.05f, 0.05f, 0.05f}));
  ogre_blue.add_component(make_shared<Color>(Vec3{.5f, .5f, 1.f}));

  auto color_system = make_shared<ChangeColorSystem>();
  auto rotation_system = make_shared<RotatingSystem>();
  auto render_system = make_shared<RenderSystem>();
  render_system->buffer(ogre);

  world.systems.push_back(render_system);
  world.systems.push_back(rotation_system);
  world.systems.push_back(color_system);
}

Application::~Application() {}

}  // namespace nycatech