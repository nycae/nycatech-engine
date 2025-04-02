//
// Created by rplaz on 2025-03-28.
//

#include "resource_manager.h"

namespace nycatech {

void ResourceManager::ClearModels()
{
  for (auto&  Model : Models) {
    for (auto& Mesh : Model.Meshes) {
      Mesh.Unload();
    }
  }
}

void ResourceManager::LoadModelFromString(const String& Content)
{
  Model::FromString(Content, Models.emplace_back());
}

void ResourceManager::LoadModelFromFile(const String& Path)
{
  Model::FromFile(Path, Models.emplace_back());
}

}  // namespace nycatech