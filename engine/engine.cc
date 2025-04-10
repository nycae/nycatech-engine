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

  auto LastFrame = Time::now();
  auto TimeDelta = duration_cast<ms>(Time::now() - LastFrame);

  while (true) {
    TimeDelta = duration_cast<ms>(Time::now() - LastFrame);
    LastFrame = Time::now();
    SDL_PollEvent(&event);
    if (event.type == SDL_EVENT_QUIT) break;
    Renderer.AddLight(LightSources);
    Renderer.Render(ResourceManager.Models);
    Renderer.MainCamera.Transform.WorldRotate({ 0, 0, 0 }, { 0.0, 0.2, 0.0 });
    String Title("NycaTech - FPS: " + std::to_string(1000.0f / TimeDelta.count()));
    SDL_SetWindowTitle(Renderer.Window, Title.c_str());
  }
}

Application::Application()
    : Renderer(), ResourceManager()
{
  // ResourceManager.LoadModelFromFile("../../../assets/teapot.glb");
  ResourceManager.LoadModelFromFile("../../../assets/knight.glb");
  //  ResourceManager.LoadModelFromFile("../../../assets/barbarian.glb");
  //  ResourceManager.LoadModelFromFile("../../../assets/mage.glb");
  //  ResourceManager.LoadModelFromFile("../../../assets/rogue.glb");
  //  ResourceManager.LoadModelFromFile("../../../assets/rogue_hooded.glb");
  LightSources.Add({ { 0.0, -20.0, -20.0 }, { 1.0, 0.9, 0.7 }, 1, 0.0, Lights::Type::Directional });
  LightSources.Add({ { 0.0, 0.0, -20.0 }, { 1.0, 0.6, 0.3 }, 2, 10, Lights::Type::Point });
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