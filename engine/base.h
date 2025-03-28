//
// Created by rplaz on 2025-02-01.
//

#ifndef NYCA_TECH_BASE_H
#define NYCA_TECH_BASE_H

#define _USE_MATH_DEFINES

#include <algorithm>
#include <array>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <cmath>
#include <cstdint>
#include <exception>
#include <fstream>
#include <glm/glm.hpp>
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

using boost::asio::post;
using std::dynamic_pointer_cast;
using std::getline;
using std::make_pair;
using std::make_shared;
using std::make_tuple;
using std::min;
using std::move;
using std::replace;
using std::static_pointer_cast;

using Uint8 = uint8_t;
using Byte = Uint8;
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
using StringReader = std::istringstream;
using FileWritter = std::ofstream;
using FileReader = std::ifstream;
using ThreadPool = boost::asio::thread_pool;

template <typename T, typename K>
using Pair = std::pair<T, K>;

template <typename T>
using SystemVector = std::vector<T>;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using Set = std::unordered_set<T>;

template <typename K, typename V>
using MultiMap = std::unordered_multimap<K, V>;

template <typename K, typename V>
using Map = std::unordered_map<K, V>;

template <typename T>
using SmartPtr = std::shared_ptr<T>;

template <typename T, Uint64 size>
using Array = std::array<T, size>;

template <typename... Ts>
using Tuple = std::tuple<Ts...>;

using Mat4 = glm::mat4;
using Vec4 = glm::vec4;
using Vec3 = glm::vec3;
using Vec2 = glm::vec2;
using Quat = glm::quat;

}  // namespace nycatech

#endif  // NYCA_TECH_BASE_H
