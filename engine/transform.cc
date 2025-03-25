//
// Created by rplaz on 2025-02-01.
//

#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace nycatech {

void Transform::translate(Vec3 pos)
{
  transform_matrix = glm::translate(transform_matrix, pos);
}

void Transform::rotate(Vec3 rot)
{
  transform_matrix = glm::rotate(transform_matrix, glm::radians(rot.x), Vec3(1.0f, 0.0f, 0.0f));
  transform_matrix = glm::rotate(transform_matrix, glm::radians(rot.y), Vec3(0.0f, 1.0f, 0.0f));
  transform_matrix = glm::rotate(transform_matrix, glm::radians(rot.z), Vec3(0.0f, 0.0f, 1.0f));
}

void Transform::scale(Vec3 scale)
{
  transform_matrix = glm::scale(transform_matrix, scale);
}

Transform::Transform(Vec3 position, Vec3 rotation, Vec3 scale)
    : transform_matrix(Mat4(1.0f))
{
  Self.scale(scale);
  Self.rotate(rotation);
  Self.translate(position);
}

Mat4 Transform::view_matrix()
{
  return glm::inverse(transform_matrix);
}

float* Transform::data()
{
  return glm::value_ptr(transform_matrix);
}

}  // namespace nycatech
