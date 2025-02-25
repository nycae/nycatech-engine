//
// Created by rplaz on 2025-01-30.
//

#include "engine.h"

#include <chrono>

#include "renderer.h"

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
  typedef std::chrono::system_clock Time;
  typedef std::chrono::milliseconds ms;
  typedef std::chrono::duration<float> fsec;

  auto last_frame = Time::now();
  while (true) {
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) break;
    auto time_delta = std::chrono::duration_cast<ms>(Time::now() - last_frame);
    scene->update(time_delta.count() / 1000.f);
  }
  SceneFactory::serialize_scene(scene, "../../../assets/scene.fb");
}

Application::Application()
    : scene(SceneFactory::deserialize_scene("main", "../../../assets/scene.fb")) {
//    : scene(SceneFactory::create_scene("main")) {
  auto ogre = MeshFactory::intance().from_file("robot", "../../../assets/robot.obj");
  if (!ogre) {
    fprintf(stderr, "unable to load model");
  }
  auto render_system = dynamic_cast<RenderSystem*>(scene->world.systems[0].get());
  render_system->buffer(ogre);
//  auto& ogre_blue = scene->create_entity();
//  ogre_blue.add_component(make_shared<MeshComponent>("robot"));
//  ogre_blue.add_component(make_shared<Transform>(Vec3{-0.f, -0.6f, 0.f}, Vec3{0.f, 0.f, 0.f}, Vec3{0.05f, 0.05f, 0.05f}));
//  ogre_blue.add_component(make_shared<Color>(Vec3{.5f, .5f, 1.f}));
//
//  auto color_system = ;
//  auto rotation_system = ;
//  auto render_system = make_shared<RenderSystem>();
//  render_system->buffer(ogre);
//
//  scene->add_system(render_system);
  scene->add_system(make_shared<RotatingSystem>());
  scene->add_system(make_shared<ChangeColorSystem>());
}

Application::~Application() {}

}  // namespace nycatech