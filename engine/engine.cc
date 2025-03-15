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
    for (auto& [transform, _] : world.entities_with<Transform, MeshComponent>()) {
      transform->rotate({0.f, 1.f, 0.f});
    }
  };
};

void Application::run() {
  typedef std::chrono::system_clock Time;
  typedef std::chrono::milliseconds ms;
  typedef std::chrono::duration<float> fsec;

  auto last_frame = Time::now();
  while (true) {
    SDL_PollEvent(&event);
    if (event.type == SDL_EVENT_QUIT) {
      break;
    }
    auto time_delta = std::chrono::duration_cast<ms>(Time::now() - last_frame);
    scene->update(time_delta.count() / 1000.f);
  }
  SceneFactory::serialize_scene(scene, "../../../assets/scene.fb");
}

Application::Application() : scene(SceneFactory::create_scene("main")) {
  post(tp, []() {
    assert(MeshFactory::instance().from_file("robot", "../../../assets/robot.obj"));
  });

  post(tp, []() {
    assert(MeshFactory::instance().from_file("ogre", "../../../assets/suzane.obj"));
  });

  post(tp, []() {
    assert(ShaderFactory::instance()
               .create_program()
               .with_shader(ShaderFactory::instance()
                                .create_shader()
                                .with_type(Shader::Type::Fragment)
                                .from_file("../../../shaders/simple_frag.glsl")
                                .with_name("simple_frag")
                                .build())
               .with_shader(ShaderFactory::instance()
                                .create_shader()
                                .with_type(Shader::Type::Vertex)
                                .from_file("../../../shaders/simple_vert.glsl")
                                .with_name("simple_vert")
                                .build())
               .with_name("simple_program")
               .build());
  });
  tp.join();

  auto render_system = make_shared<RenderSystem>();
  auto& entity_ogre = scene->world.create_entity();
  entity_ogre.add_component<MeshComponent>("ogre");
  entity_ogre.add_component<Transform>(Vec3{-0.f, -0.6f, 0.f}, Vec3{0.f, 0.f, 0.f}, Vec3{0.05f, 0.05f, 0.05f});
  entity_ogre.add_component<ShaderComponent>("simple_program");

  auto& entity_camera = scene->world.create_entity();
  entity_camera.add_component<Transform>(Vec3{-1.0f, -0.5f, -1.5f}, Vec3{}, Vec3{.5f, .5f, .5f});
  entity_camera.add_component<Camera>(180.0f, 1600.0f / 900.0f, 0.1f, 1000.0f, true);

  scene->add_system(render_system);
  scene->add_system(make_shared<RotatingSystem>());
}

Application::~Application() {}

}  // namespace nycatech