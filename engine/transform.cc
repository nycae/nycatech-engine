//
// Created by rplaz on 2025-02-01.
//

#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace nycatech {

void Transform::Translate(Vec3 pos)
{
  TransformMatrix = glm::translate(TransformMatrix, pos);
}

void Transform::Rotate(Vec3 rot)
{
  TransformMatrix = glm::rotate(TransformMatrix, glm::radians(rot.x), Vec3(1.0f, 0.0f, 0.0f));
  TransformMatrix = glm::rotate(TransformMatrix, glm::radians(rot.y), Vec3(0.0f, 1.0f, 0.0f));
  TransformMatrix = glm::rotate(TransformMatrix, glm::radians(rot.z), Vec3(0.0f, 0.0f, 1.0f));
}

void Transform::Scale(Vec3 scale)
{
  TransformMatrix = glm::scale(TransformMatrix, scale);
}

Transform::Transform(Vec3 position, Vec3 rotation, Vec3 scale)
    : TransformMatrix(Mat4(glm::identity<Mat4>()))
{
  Self.Scale(scale);
  Self.Rotate(rotation);
  Self.Translate(position);
}

Mat4 Transform::ViewMatrix() const
{
  return glm::inverse(TransformMatrix);
}

float* Transform::Data()
{
  return glm::value_ptr(TransformMatrix);
}

const Float32* Transform::Data() const
{
  return Data();
}

void Transform::WorldRotate(Vec3 Point, Vec3 Angle)
{
  Vec3 current_pos = Vec3(TransformMatrix[3]);
  Mat4 translation_to_pivot = glm::translate(glm::mat4(1.0f), -Point);
  Mat4 rotation = glm::mat4(1.0f);
  rotation = glm::rotate(rotation, glm::radians(Angle.x), Vec3(1.0f, 0.0f, 0.0f));
  rotation = glm::rotate(rotation, glm::radians(Angle.y), Vec3(0.0f, 1.0f, 0.0f));
  rotation = glm::rotate(rotation, glm::radians(Angle.z), Vec3(0.0f, 0.0f, 1.0f));
  Mat4 translation_from_pivot = glm::translate(glm::mat4(1.0f), Point);
  TransformMatrix = translation_from_pivot * rotation * translation_to_pivot * TransformMatrix;
}

}  // namespace nycatech
