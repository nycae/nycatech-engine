//
// Created by rplaz on 2025-03-28.
//

#pragma once

#include "base.h"
#include "renderer.h"

namespace nycatech {

class ResourceManager {
public:
  void LoadModelFromString(const String& Content);
  void LoadModelFromFile(const String& Path);
  void ClearModels();

public:
  Vector<Model> Models;
};

}  // namespace nycatech
