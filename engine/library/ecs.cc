//
// Created by rplaz on 2025-02-23.
//

#include "ecs.h"

namespace nycatech {

Entity& World::CreateEntity() {
  Entity newEntity;
  entities.PushBack(std::move(newEntity));
  return entities[entities.Size() - 1];
}

void World::Tick(Float32 timeDelta) {
  for (auto& system : systems) {
    system->Tick(Self, timeDelta);
  }
}


}  // namespace nycatech