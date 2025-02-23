//
// Created by rplaz on 2025-02-01.
//

#ifndef NYCA_TECH_BASE_H
#define NYCA_TECH_BASE_H

#include <cstdint>
#include <exception>
#include <sstream>
#include <string>
#include <system_error>
#include <typeindex>

namespace nycatech {

#ifndef Self
#define Self (*this)
#endif

#ifndef INLINE
#define INLINE inline
#endif

using Uint8 = uint8_t;
using Uint16 = uint16_t;
using Uint32 = uint32_t;
using Uint64 = uint64_t;
using Int32 = int32_t;
using Int64 = int64_t;
using Float32 = float;
using Float64 = double;
using String = std::string;
using Exception = std::exception;
using RuntimeError = std::runtime_error;
using StringBuilder = std::stringstream;
using FileWritter = std::ofstream;
using FileReader = std::ifstream;
using Type = std::type_index;

template <typename T, typename K>
class Pair {
 public:
  T first;
  K second;
};

template <typename T>
using SmartPtr = std::shared_ptr<T>;

}  // namespace nycatech

#endif  // NYCA_TECH_BASE_H
