//
// Created by rplaz on 2025-01-30.
//

#include "model.h"

namespace nycatech::render {

void Model::Translate(Array<float, 3> pos) {
  transform.position[0] += pos[0];
  transform.position[1] += pos[1];
  transform.position[2] += pos[2];
}

void Model::Rotate(Array<float, 3> rot) {
  transform.rotation[0] += rot[0];
  transform.rotation[1] += rot[1];
  transform.rotation[2] += rot[2];
}

void Model::Scale(Array<float, 3> scale) {
  transform.scale[0] *= scale[0];
  transform.scale[1] *= scale[1];
  transform.scale[2] *= scale[2];
}

}  // namespace nycatech::render