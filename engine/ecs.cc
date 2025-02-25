//
// Created by rplaz on 2025-02-23.
//

#include "ecs.h"

namespace nycatech {

Entity& World::create_entity() {
  Entity entity;
  entities.push_back(std::move(entity));
  return entities.back();
}

void World::tick(Float32 time_delta) {
  for (auto& system : systems) {
    system->tick(Self, time_delta);
  }
}

}  // namespace nycatech