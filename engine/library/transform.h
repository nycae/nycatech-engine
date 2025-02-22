//
// Created by rplaz on 2025-02-01.
//

#ifndef NYCA_TECH_TRANSFORM_H
#define NYCA_TECH_TRANSFORM_H

#include "base.h"
#include "array.h"

namespace nycatech {

struct Transform {
  Array<Float32, 3> position;
  Array<Float32, 3> rotation;
  Array<Float32, 3> scale;
};

}  // namespace nycatech

#endif  // NYCA_TECH_TRANSFORM_H
