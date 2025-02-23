//
// Created by rplaz on 2025-01-30.
//

#include "engine.h"

#include "renderer/renderer.h"

namespace nycatech {

using namespace render;

class RotatingSystem final : public System {
 public:
  void Tick(World& world, Float32 timeDelta) override {
    for (auto entity : world.EntitiesWithComponent<Transform>()) {
      auto [transform] = entity->Get<Transform>();
      transform->Rotate({0.f, 1.f, 0.f});
    }
  };
};

void Application::Run() {
  while (true) {
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) break;
    world.Tick(0.f);
  }
}

Application::Application() : world() {
  auto ogre = render::Mesh::FromFile("../../../assets/ogre.obj");
  if (!ogre) {
    fprintf(stderr, "unable to load model");
  }

  auto& redOgreEntity = world.CreateEntity();
  redOgreEntity.AddComponent(ogre);
  redOgreEntity.AddComponent(Transform::Make({0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.1f, 0.1f, 0.1f}));
  redOgreEntity.AddComponent(SmartPtr<Color>(new Color({1.f, .5, .5})));

  auto& blueOgreEntity = world.CreateEntity();
  blueOgreEntity.AddComponent(ogre);
  blueOgreEntity.AddComponent(Transform::Make({0.f, -0.5f, 0.f}, {0.f, 0.f, 0.f}, {0.1f, 0.1f, 0.1f}));
  blueOgreEntity.AddComponent(SmartPtr<Color>(new Color({.5f, .5f, 1.f})));

  auto& greenOgreEntity = world.CreateEntity();
  greenOgreEntity.AddComponent(ogre);
  greenOgreEntity.AddComponent(Transform::Make({0.f, -1.0f, 0.f}, {0.f, 0.f, 0.f}, {0.1f, 0.1f, 0.1f}));
  greenOgreEntity.AddComponent(SmartPtr<Color>(new Color({.5f, 1.f, .5f})));

  auto rotationSystem = std::make_shared<RotatingSystem>();
  auto renderSystem = std::make_shared<RenderSystem>();
  renderSystem->Buffer(ogre);

  world.systems.PushBack(renderSystem);
  world.systems.PushBack(rotationSystem);
}

Application::~Application() {}

}  // namespace nycatech