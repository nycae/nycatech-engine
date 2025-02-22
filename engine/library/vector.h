//
// Created by rplaz on 2025-02-01.
//

#ifndef NYCA_TECH_VECTOR_H
#define NYCA_TECH_VECTOR_H

#include <functional>
#include <vector>

#include "base.h"

namespace nycatech {

template <typename T>
class Vector {
 public:
  INLINE explicit Vector(Uint64 count = 0);
  INLINE explicit Vector(std::initializer_list<T> init);

  INLINE Vector(Vector&& other);
  INLINE Vector(const Vector& other);

  INLINE ~Vector();

  INLINE Vector& operator=(Vector&& other);
  INLINE Vector& operator=(const Vector& other);
  INLINE Vector& operator=(std::vector<T>);

 public:
  INLINE bool Resize(Uint64 newSize);
  INLINE bool Insert(const T& element);
  INLINE bool At(Uint64 index, T* elem) const;
  INLINE bool At(Uint64 index, T** elem) const;
  INLINE bool Emplace(Uint64 index, const T& elem);
  INLINE bool AdjustSize();
  INLINE T& operator[](Uint64 index);
  INLINE const T& operator[](Uint64 index) const;
  INLINE Uint64 Capacity() const;
  INLINE Uint64& CountMut();
  INLINE Uint64 Count() const;
  INLINE void OverrideCount(Uint64 newSize);
  INLINE T* Data();
  INLINE const T* Data() const;
  INLINE bool Contains(const T& other) const;
  INLINE bool Contains(std::function<bool(const T&)>) const;
  INLINE bool IsEmpty() const;
  INLINE Int32 ElemSize() const;

 public:
  INLINE T* begin();
  INLINE const T* begin() const;
  INLINE T* end();
  INLINE const T* end() const;

 private:
  T* data;
  Uint64 count;
  Uint64 size;
};

template <typename T>
INLINE Vector<T>::~Vector() {
  delete[] data;
  data = nullptr;
}

template <typename T>
INLINE Vector<T>::Vector(Uint64 initCount)
    : size(initCount), count(initCount), data(new T[initCount]) {}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> init) : data(nullptr), count(0), size(0) {
  for (const auto& elem : init) {
    this->Insert(elem);
  }
}

template <typename T>
Vector<T>::Vector(Vector&& other) : data(other.data), count(other.count), size(other.size) {
  other.data = nullptr;
  other.count = 0;
  other.size = 0;
}

template <typename T>
Vector<T>::Vector(const Vector& other) : data(new T[other.size]), count(other.count), size(other.size) {
  std::copy(other.data, other.data + other.count, data);
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) {
  if (this == &other) return *this;
  delete[] data;
  data = other.data;
  count = other.count;
  size = other.size;
  other.data = nullptr;
  other.size = 0;
  other.count = 0;
  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
  if (this == &other) return *this;
  delete[] data;
  data = new T[other.size];
  std::copy(other.data, other.data + other.count, data);
  size = other.size;
  count = other.count;
  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(std::vector<T> other) {
  delete[] data;
  data = new T[other.capacity()];
  std::copy(other.begin(), other.end(), data);
  count = other.size();
  size = other.capacity();
  return *this;
}

template <typename T>
INLINE bool Vector<T>::Insert(const T& element) {
  if (count >= size) {
    const Uint64 newCapacity = size * 1.62;
    if (!Resize(newCapacity + (16 - (newCapacity % 16)))) return false;
  }
  new (&data[count]) T(element);
  count++;
  return true;
}

template <typename T>
INLINE bool Vector<T>::At(Uint64 index, T* elem) const {
  if (index >= count) {
    return false;
  }

  *elem = data[index];
  return true;
}

template <typename T>
bool Vector<T>::At(Uint64 index, T** elem) const {
  if (index >= count) {
    return false;
  }

  *elem = &data[index];
  return true;
}

template <typename T>
bool Vector<T>::Emplace(Uint64 index, const T& elem) {
  if (index >= count) {
    return false;
  }
  data[index] = elem;
  return true;
}

template <typename T>
bool Vector<T>::AdjustSize() {
  return Resize(count);
}

template <typename T>
T& Vector<T>::operator[](Uint64 index) {
  return data[index];
}

template <typename T>
const T& Vector<T>::operator[](Uint64 index) const {
  return data[index];
}

template <typename T>
Uint64 Vector<T>::Capacity() const {
  return size;
}

template <typename T>
Uint64 Vector<T>::Count() const {
  return count;
}

template <typename T>
void Vector<T>::OverrideCount(Uint64 newSize) {
  count = newSize;
}

template <typename T>
Uint64& Vector<T>::CountMut() {
  return count;
}

template <typename T>
T* Vector<T>::Data() {
  return data;
}

template <typename T>
const T* Vector<T>::Data() const {
  return data;
}

template <typename T>
bool Vector<T>::Contains(const T& other) const {
  for (Int32 i = 0; i < count; i++) {
    if (data[i] == other) {
      return true;
    }
  }
  return false;
}

template <typename T>
bool Vector<T>::Contains(std::function<bool(const T&)> found) const {
  for (Int32 i = 0; i < count; i++) {
    if (found(data[i])) {
      return true;
    }
  }
  return false;
}

template <typename T>
bool Vector<T>::IsEmpty() const {
  return count <= 0;
}

template <typename T>
Int32 Vector<T>::ElemSize() const {
  return sizeof(T);
}

template <typename T>
T* Vector<T>::begin() {
  return data;
}

template <typename T>
const T* Vector<T>::begin() const {
  return data;
}

template <typename T>
T* Vector<T>::end() {
  return data + count;
}

template <typename T>
const T* Vector<T>::end() const {
  return data + count;
}

template <typename T>
INLINE bool Vector<T>::Resize(const Uint64 newSize) {
  if (newSize < count) {
    return false;
  }
  if (newSize == size) {
    return true;
  }
  T* newData = new T[newSize];
  std::copy(data, data + count, newData);
  delete[] data;
  data = newData;
  size = newSize;
  return true;
}

}  // namespace nycatech

#endif  // NYCA_TECH_VECTOR_H
