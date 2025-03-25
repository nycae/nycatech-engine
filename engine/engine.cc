//
// Created by rplaz on 2025-01-30.
//

#include "engine.h"

#include <chrono>
#include <iostream>

#include "renderer.h"

namespace nycatech {

class RotatingSystem final : public System {
public:
  void tick(World &world, Float32 time_delta) override
  {
    for (auto &[_, transform] : world.entities_with<Mesh, Transform>()) {
      transform->scale({ 1.1, 1.1, 1.1 });
      transform->translate({0, 0, -0.1});
    }
  };
};

void Application::run()
{
  typedef std::chrono::system_clock    Time;
  typedef std::chrono::milliseconds    ms;
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

Application::Application(SmartPtr<Scene> scene)
    : scene(scene)
{
  auto  renderer = make_shared<Renderer>();
  auto &entity_ogre = scene->world.create_entity();
  entity_ogre.add_component(Mesh::from_file("../../../assets/knight.glb"));
  entity_ogre.add_component(make_shared<Transform>(Vec3{ 0.f, 0.f, 0.f }, Vec3{ 0.0f, 0.0f, 0.0f }, Vec3{ 0.05f, 0.05f, 0.05f }));
  entity_ogre.add_component(Texture::from_file("../../../assets/knight_texture.png"));
  entity_ogre.add_component(ShaderFactory::create_program()
                                .with_shader(ShaderFactory::create_shader()
                                                 .with_type(Shader::Type::Fragment)
                                                 .from_file("../../../shaders/camera_frag.glsl")
                                                 .with_name("camera_frag")
                                                 .build())
                                .with_shader(ShaderFactory::create_shader()
                                                 .with_type(Shader::Type::Vertex)
                                                 .from_file("../../../shaders/camera_vert.glsl")
                                                 .with_name("camera_vert")
                                                 .build())
                                .with_name("simple_program")
                                .build());

  scene->add_system(renderer);
  scene->add_system(make_shared<RotatingSystem>());
}

Application::~Application()
{
}

}  // namespace nycatech