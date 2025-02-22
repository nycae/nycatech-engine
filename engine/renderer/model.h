//
// Created by rplaz on 2025-01-30.
//

#ifndef NYCA_TECH_MODEL_H
#define NYCA_TECH_MODEL_H

#include "mesh.h"

namespace nycatech::render {

class Model {
 public:
  Mesh *mesh;
  Transform transform;
  Array<float, 3> color;

 public:
  void Translate(Array<float, 3> pos);
  void Rotate(Array<float, 3> rot);
  void Scale(Array<float, 3> scale);
};

}  // namespace nycatech::render

#endif  // NYCA_TECH_MODEL_H
