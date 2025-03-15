//
// Created by rplaz on 2025-02-01.
//

#ifndef NYCA_TECH_BASE_H
#define NYCA_TECH_BASE_H

#define _USE_MATH_DEFINES

#include <array>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <cmath>
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

using boost::asio::post;
using std::dynamic_pointer_cast;
using std::getline;
using std::make_pair;
using std::make_shared;
using std::make_tuple;
using std::min;
using std::move;
using std::static_pointer_cast;

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
using StringReader = std::istringstream;
using FileWritter = std::ofstream;
using FileReader = std::ifstream;
using Type = Uint64;
using ThreadPool = boost::asio::thread_pool;

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

template <typename... Ts>
using Tuple = std::tuple<Ts...>;

typedef Array<Float32, 3> Vec3;
typedef Array<Float32, 2> Vec2;

template <Uint64 H, Uint64 W>
class Matrix {
 public:
  Matrix(const Array<Float32, H * W>& init = {}) : inner(init) {};

  static constexpr Matrix identity() {
    Matrix matrix;
    for (auto i = 0ul; i < min(H, W); i++) matrix.at(i, i) = 1;
    return matrix;
  }

  Float32& at(Uint64 i, Uint64 j) {
    return inner.at(i * W + j);
  };

  const Float32& at(Uint64 i, Uint64 j) const {
    return inner.at(i * W + j);
  };

  Matrix& operator=(const Matrix& other) = default;

  Matrix operator+(const Matrix& other) {
    Matrix m;
    for (auto i = 0ul; i < W; i++) {
      for (auto j = 0ul; j < H; j++) {
        m.at(i, j) = Self.at(i, j) + other.at(i, j);
      }
    }
    return m;
  };

  template <Uint64 I, Uint64 J>
  Matrix<H, J> operator*(const Matrix<I, J>& other) {
    static_assert(W == I);
    Matrix<H, J> m;
    for (auto i = 0ul; i < H; i++) {
      for (auto j = 0ul; j < J; j++) {
        for (auto k = 0ul; k < I; k++) {
          m.at(i, j) += Self.at(i, k) * other.at(k, j);
        }
      }
    }
    return m;
  }

 private:
  Array<Float32, H * W> inner;
};

Vec3 normalize(const Vec3& v);
Vec3 cross(const Vec3& a, const Vec3& b);
float dot(const Vec3& a, const Vec3& b);

using TransformMatrix = Matrix<4, 4>;

}  // namespace nycatech

#endif  // NYCA_TECH_BASE_H
