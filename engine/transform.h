//
// Created by rplaz on 2025-02-01.
//

#pragma once

#include "base.h"

namespace nycatech {

struct Transform {
  Transform(Vec3 position = {}, Vec3 rotation = {}, Vec3 scale = { 1.f, 1.f, 1.f });

  Float32*       Data();
  const Float32* Data() const;
  Mat4           ViewMatrix() const;

  void Translate(Vec3 pos);
  void Rotate(Vec3 rot);
  void Scale(Vec3 scale);

  Mat4 TransformMatrix;
};

}  // namespace nycatech
