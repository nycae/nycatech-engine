//
// Created by rplaz on 2025-02-01.
//

#ifndef NYCA_TECH_ARRAY_H
#define NYCA_TECH_ARRAY_H

#include <array>

#include "base.h"

namespace nycatech {

template <typename T, Int32 S>
using Array = std::array<T, S>;

}

#endif  // NYCA_TECH_ARRAY_H
