#ifndef NYCA_TECH_GAME_VECTOR_H
#define NYCA_TECH_GAME_VECTOR_H

#include <cstring>
#include <utility>

#include "base.h"

namespace nycatech {

template <typename T, Uint64 InlineCapacity = 16>
class Vector {
 public:
  INLINE Vector();
  INLINE ~Vector();

  INLINE void PushBack(const T& element);
  INLINE void PushBack(T&& element);
  INLINE bool PopBack();
  INLINE T& operator[](Uint64 index);
  INLINE const T& operator[](Uint64 index) const;
  INLINE Uint64 Size() const;
  INLINE Uint64 Allocated() const;
  INLINE bool IsEmpty() const;
  INLINE void Clear();

  INLINE T* Data() { return data; }
  INLINE const T* Data() const { return data; }

  INLINE T* begin() { return data; }
  INLINE T* end() { return data + size; }

  INLINE const T* begin() const { return data; }
  INLINE const T* end() const { return data + size; }

  INLINE void GrowToSize(Uint64 newSize);

 private:
  INLINE void Grow();

 private:
  Uint64 size;
  Uint64 capacity;
  T* data;
  alignas(T) char inlineStorage[sizeof(T) * InlineCapacity];
};

template <typename T, Uint64 InlineCapacity>
INLINE Vector<T, InlineCapacity>::Vector()
    : size(0), capacity(InlineCapacity), data(reinterpret_cast<T*>(inlineStorage)) {}

template <typename T, Uint64 InlineCapacity>
INLINE Vector<T, InlineCapacity>::~Vector() {
  Clear();
  if (data != reinterpret_cast<T*>(inlineStorage)) {
    delete[] data;
  }
}

template <typename T, Uint64 InlineCapacity>
INLINE void Vector<T, InlineCapacity>::PushBack(const T& element) {
  if (size >= capacity) {
    Grow();
  }
  new (&data[size]) T(element);
  size++;
}

template <typename T, Uint64 InlineCapacity>
INLINE void Vector<T, InlineCapacity>::PushBack(T&& element) {
  if (size >= capacity) {
    Grow();
  }
  new (&data[size]) T(std::move(element));
  size++;
}

template <typename T, Uint64 InlineCapacity>
INLINE bool Vector<T, InlineCapacity>::PopBack() {
  if (size > 0) {
    size--;
    data[size].~T();
    return true;
  }
  return false;
}

template <typename T, Uint64 InlineCapacity>
INLINE T& Vector<T, InlineCapacity>::operator[](Uint64 index) {
  return data[index];
}

template <typename T, Uint64 InlineCapacity>
INLINE const T& Vector<T, InlineCapacity>::operator[](Uint64 index) const {
  return data[index];
}

template <typename T, Uint64 InlineCapacity>
INLINE Uint64 Vector<T, InlineCapacity>::Size() const {
  return size;
}

template <typename T, Uint64 InlineCapacity>
INLINE Uint64 Vector<T, InlineCapacity>::Allocated() const {
  return capacity;
}

template <typename T, Uint64 InlineCapacity>
INLINE bool Vector<T, InlineCapacity>::IsEmpty() const {
  return size == 0;
}

template <typename T, Uint64 InlineCapacity>
INLINE void Vector<T, InlineCapacity>::Clear() {
  for (Uint64 i = 0; i < size; ++i) {
    data[i].~T();
  }
  size = 0;
}

template <typename T, Uint64 InlineCapacity>
INLINE void Vector<T, InlineCapacity>::Grow() {
  const Uint64 newCapacity = (size + 1) * 1.62;
  GrowToSize(newCapacity + (16 - (newCapacity % 16)));
}

template <typename T, Uint64 InlineCapacity>
void Vector<T, InlineCapacity>::GrowToSize(Uint64 newSize) {
  T* newData = reinterpret_cast<T*>(new char[newSize * sizeof(T)]);
  for (Uint64 i = 0; i < size; ++i) {
    new (&newData[i]) T(std::move(data[i]));
    data[i].~T();
  }
  if (data != reinterpret_cast<T*>(inlineStorage)) {
    delete[] reinterpret_cast<char*>(data);
  }
  data = newData;
  capacity = newSize;
}

}  // namespace nycatech

#endif  // NYCA_TECH_GAME_VECTOR_H
