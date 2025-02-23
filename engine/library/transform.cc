//
// Created by rplaz on 2025-02-01.
//

#include "transform.h"

namespace nycatech {

void Transform::Translate(Array<float, 3> pos) {
  position[0] += pos[0];
  position[1] += pos[1];
  position[2] += pos[2];
}

void Transform::Rotate(Array<float, 3> rot) {
  rotation[0] += rot[0];
  rotation[1] += rot[1];
  rotation[2] += rot[2];
}

void Transform::Scale(Array<float, 3> scale) {
  scale[0] *= scale[0];
  scale[1] *= scale[1];
  scale[2] *= scale[2];
}

Transform::Transform(Array<Float32, 3> position, Array<Float32, 3> rotation, Array<Float32, 3> scale)
    : position(position), rotation(rotation), scale(scale) {}
SmartPtr<Transform> Transform::Make(Array<Float32, 3> position, Array<Float32, 3> rotation, Array<Float32, 3> scale) {
  return SmartPtr<Transform>(new Transform(position, rotation, scale));
}

}  // namespace nycatech
