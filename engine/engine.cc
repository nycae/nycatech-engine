//
// Created by rplaz on 2025-01-30.
//

#include "engine.h"

#include <chrono>

#include "renderer.h"

namespace nycatech {

void Application::Run()
{
  using std::chrono::duration_cast;
  typedef std::chrono::system_clock Time;
  typedef std::chrono::milliseconds ms;

  auto last_frame = Time::now();
  while (true) {
    SDL_PollEvent(&event);
    if (event.type == SDL_EVENT_QUIT) break;
    Renderer.AddLight(LightSources);
    Renderer.Render(ResourceManager.Models);
    auto time_delta = duration_cast<ms>(Time::now() - last_frame);
  }
}

Application::Application()
    : Renderer(), ResourceManager()
{
  ResourceManager.LoadModelFromFile("../../../assets/knight.glb");
  //  ResourceManager.LoadModelFromFile("../../../assets/barbarian.glb");
  //  ResourceManager.LoadModelFromFile("../../../assets/mage.glb");
  //  ResourceManager.LoadModelFromFile("../../../assets/rogue.glb");
  //  ResourceManager.LoadModelFromFile("../../../assets/rogue_hooded.glb");
  LightSources.Add({ { 2, 0, 2 }, { 1, 1, 1 }, 1, 1.5, Lights::Type::Point });
  LightSources.Add({ { 0, 10, 0 }, { 1, 1, 1 }, 10, 10, Lights::Type::Point });
  Renderer.ModelShader = ShaderFactory::CreateProgram()
                             .WithShader(ShaderFactory::CreateShader()
                                             .FromFile("../../../shaders/model_vert.glsl")
                                             .WithType(GL_VERTEX_SHADER)
                                             .Build())
                             .WithShader(ShaderFactory::CreateShader()
                                             .FromFile("../../../shaders/model_frag.glsl")
                                             .WithType(GL_FRAGMENT_SHADER)
                                             .Build())
                             .Build();
}

}  // namespace nycatech