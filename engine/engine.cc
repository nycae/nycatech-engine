//
// Created by rplaz on 2025-01-30.
//

#include "engine.h"

#include <chrono>

#include "renderer.h"

namespace nycatech {

class RotatingSystem final : public System {
 public:
  void tick(World& world, Float32 time_delta) override {
    for (auto& [transform] : world.entities_with<Transform>()) {
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
    for (auto& [color] : world.entities_with<Color>()) {
      if (!color) continue;
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
    if (event.type == SDL_EVENT_QUIT) break;
    auto time_delta = std::chrono::duration_cast<ms>(Time::now() - last_frame);
    scene->update(time_delta.count() / 1000.f);
  }
  SceneFactory::serialize_scene(scene, "../../../assets/scene.fb");
}

Application::Application() : scene(SceneFactory::create_scene("main")) {
  post(tp, []() {
    auto robot = MeshFactory::instance().from_file("robot", "../../../assets/robot.obj");
    if (!robot) fprintf(stderr, "unable to load model");
  });
  post(tp, []() {
    auto ogre = MeshFactory::instance().from_file("ogre", "../../../assets/ogre.obj");
    if (!ogre) fprintf(stderr, "unable to load model");
  });
  tp.join();

  auto render_system = make_shared<RenderSystem>();
  auto& entity_ogre = scene->world.create_entity();
  entity_ogre.add_component<MeshComponent>("ogre");
  entity_ogre.add_component<Transform>(Vec3{-0.f, -0.6f, 0.f}, Vec3{0.f, 0.f, 0.f}, Vec3{0.05f, 0.05f, 0.05f});
  entity_ogre.add_component<Color>(Vec3{0.1, 0.3, 0.7});

  auto& entity_camera = scene->world.create_entity();
  entity_camera.add_component<Transform>();
  entity_camera.add_component<Camera>();

  scene->add_system(render_system);
  scene->add_system(make_shared<RotatingSystem>());
  scene->add_system(make_shared<ChangeColorSystem>());
}

Application::~Application() {}

}  // namespace nycatech