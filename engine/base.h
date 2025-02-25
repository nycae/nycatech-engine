//
// Created by rplaz on 2025-02-01.
//

#ifndef NYCA_TECH_BASE_H
#define NYCA_TECH_BASE_H

#include <array>
#include <cstdint>
#include <exception>
#include <fstream>
#include <sstream>
#include <string>
#include <system_error>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>

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
using Pair = std::pair<T, K>;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using Set = std::unordered_set<T>;

template <typename K, typename V>
using Map = std::unordered_map<K, V>;

template <typename T>
using SmartPtr = std::shared_ptr<T>;

template <typename T, Uint64 size>
using Array = std::array<T, size>;

typedef Array<Float32, 3> Vec3;

using std::make_shared;
using std::make_tuple;
using std::move;
using std::make_pair;

}  // namespace nycatech

#endif  // NYCA_TECH_BASE_H
