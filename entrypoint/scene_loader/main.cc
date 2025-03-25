//
// Created by rplaz on 2025-03-24.
//

#include <engine/engine.h>
#include <iostream>

using namespace nycatech;

int main() {
  auto scene = SceneFactory::deserialize_scene("main", "../../../assets/scene.fb");
  Application app(scene);
  app.run();
}