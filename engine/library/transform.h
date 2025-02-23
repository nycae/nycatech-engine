//
// Created by rplaz on 2025-02-01.
//

#ifndef NYCA_TECH_TRANSFORM_H
#define NYCA_TECH_TRANSFORM_H

#include "array.h"
#include "base.h"
#include "ecs.h"

namespace nycatech {

class Transform : public Component {
 public:
  Transform(Array<Float32, 3> position = {0.f, 0.f, 0.f},
            Array<Float32, 3> rotation = {0.f, 0.f, 0.f},
            Array<Float32, 3> scale = {1.f, 1.f, 1.f});

  static SmartPtr<Transform> Make(Array<Float32, 3> position = {0.f, 0.f, 0.f},
                                  Array<Float32, 3> rotation = {0.f, 0.f, 0.f},
                                  Array<Float32, 3> scale = {1.f, 1.f, 1.f});

 public:
  void Translate(Array<float, 3> pos);
  void Rotate(Array<float, 3> rot);
  void Scale(Array<float, 3> scale);

 public:
  Array<Float32, 3> position;
  Array<Float32, 3> rotation;
  Array<Float32, 3> scale;
};

}  // namespace nycatech

#endif  // NYCA_TECH_TRANSFORM_H
