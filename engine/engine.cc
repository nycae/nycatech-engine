//
// Created by rplaz on 2025-01-30.
//

#include "engine.h"

#include <chrono>
#include <iostream>

#include "renderer.h"

namespace nycatech {

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
    Renderer.Render({ Model });
    auto time_delta = std::chrono::duration_cast<ms>(Time::now() - last_frame);
  }
}

Application::Application()
    : Renderer()
{
  Model = Model::FromFile("../../../assets/knight.glb");
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

Application::~Application()
{
}

}  // namespace nycatech