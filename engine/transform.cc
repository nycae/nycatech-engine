//
// Created by rplaz on 2025-02-01.
//

#include "transform.h"

namespace nycatech {

void Transform::translate(Vec3 pos) {
  position[0] += pos[0];
  position[1] += pos[1];
  position[2] += pos[2];
}

void Transform::rotate(Vec3 rot) {
  rotation[0] += rot[0];
  rotation[1] += rot[1];
  rotation[2] += rot[2];
}

void Transform::scale(Vec3 scale) {
  Self.local_scale[0] *= scale[0];
  Self.local_scale[1] *= scale[1];
  Self.local_scale[2] *= scale[2];
}

Transform::Transform(Vec3 position, Vec3 rotation, Vec3 scale)
    : position(position), rotation(rotation), local_scale(scale) {}

SmartPtr<Transform> Transform::Make(Vec3 position, Vec3 rotation, Vec3 scale) {
  return make_shared<Transform>(position, rotation, scale);
}

}  // namespace nycatech
