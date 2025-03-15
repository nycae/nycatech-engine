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

TransformMatrix Transform::transform_matrix() {
  auto translation_matrix = TransformMatrix::identity();
  translation_matrix.at(3, 0) = position[0];
  translation_matrix.at(3, 1) = position[1];
  translation_matrix.at(3, 2) = position[2];

  auto scale_matrix = TransformMatrix::identity();
  scale_matrix.at(3, 0) = local_scale[0];
  scale_matrix.at(3, 1) = local_scale[1];
  scale_matrix.at(3, 2) = local_scale[2];

  auto rotx_matrix = TransformMatrix::identity();
  const auto rad_x = rotation[0] * 3.14159265358979323846f / 180.f;
  rotx_matrix.at(1, 1) = cos(rad_x);
  rotx_matrix.at(1, 2) = -sin(rad_x);
  rotx_matrix.at(2, 1) = sin(rad_x);
  rotx_matrix.at(2, 2) = cos(rad_x);

  auto roty_matrix = TransformMatrix::identity();
  const auto rad_y = rotation[1] * 3.14159265358979323846f / 180.f;
  roty_matrix.at(0, 0) = cos(rad_y);
  roty_matrix.at(0, 2) = sin(rad_y);
  roty_matrix.at(2, 0) = -sin(rad_y);
  roty_matrix.at(2, 2) = cos(rad_y);

  auto rotz_matrix = TransformMatrix::identity();
  const auto rad_z = rotation[1] * 3.14159265358979323846f / 180.f;
  roty_matrix.at(0, 0) = cos(rad_z);
  roty_matrix.at(0, 2) = -sin(rad_z);
  roty_matrix.at(2, 0) = sin(rad_z);
  roty_matrix.at(2, 2) = cos(rad_z);

  return translation_matrix * scale_matrix * rotx_matrix * roty_matrix * rotz_matrix;
}
TransformMatrix Transform::view_matrix() {
  const Vec3 z_axis = normalize(position);
  const Vec3 x_axis = normalize(cross({0.0f, 1.0f, 0.0f}, z_axis));
  const Vec3 y_axis = cross(z_axis, x_axis);

  auto view_matrix = TransformMatrix::identity();

  view_matrix.at(0, 0) = x_axis[0];
  view_matrix.at(1, 0) = x_axis[1];
  view_matrix.at(2, 0) = x_axis[2];

  view_matrix.at(0, 1) = y_axis[0];
  view_matrix.at(1, 1) = y_axis[1];
  view_matrix.at(2, 1) = y_axis[2];

  view_matrix.at(0, 2) = z_axis[0];
  view_matrix.at(1, 2) = z_axis[1];
  view_matrix.at(2, 2) = z_axis[2];

  view_matrix.at(3, 0) = -dot(x_axis, position);
  view_matrix.at(3, 1) = -dot(y_axis, position);
  view_matrix.at(3, 2) = -dot(z_axis, position);

  return view_matrix;
}

}  // namespace nycatech
