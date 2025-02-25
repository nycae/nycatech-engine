//
// Created by rplaz on 2025-02-01.
//

#ifndef NYCA_TECH_TRANSFORM_H
#define NYCA_TECH_TRANSFORM_H

#include "base.h"
#include "ecs.h"

namespace nycatech {

class Transform : public Component {
 public:
  Transform(Vec3 position = {0.f, 0.f, 0.f}, Vec3 rotation = {0.f, 0.f, 0.f}, Vec3 scale = {1.f, 1.f, 1.f});

  static SmartPtr<Transform> Make(Vec3 position = {0.f, 0.f, 0.f},
                                  Vec3 rotation = {0.f, 0.f, 0.f},
                                  Vec3 scale = {1.f, 1.f, 1.f});

 public:
  void translate(Vec3 pos);
  void rotate(Vec3 rot);
  void scale(Vec3 scale);

 public:
  Vec3 position;
  Vec3 rotation;
  Vec3 local_scale;
};

}  // namespace nycatech

#endif  // NYCA_TECH_TRANSFORM_H
