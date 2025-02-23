#ifndef NYCA_TECH_GAME_HASHSET_H
#define NYCA_TECH_GAME_HASHSET_H

#include <functional>

#include "base.h"
#include "vector.h"

namespace nycatech {

template <typename K, Uint64 Capacity = 16>
class HashSet {
 public:
  INLINE HashSet();
  INLINE ~HashSet() = default;

  INLINE bool Insert(const K& key);
  INLINE bool Remove(const K& key);
  INLINE bool Contains(const K& key) const;
  INLINE Uint64 Size() const;
  INLINE bool IsEmpty() const;
  INLINE void Clear();

  INLINE K& operator[](Uint64 index);
  INLINE const K& operator[](Uint64 index) const;

  INLINE auto begin();
  INLINE auto end();
  INLINE auto begin() const;
  INLINE auto end() const;

 private:
  INLINE Uint64 HashFunction(const K& key) const;

 private:
  Vector<K> buckets[Capacity];
  Uint64 size;
};

template <typename K, Uint64 Capacity>
INLINE HashSet<K, Capacity>::HashSet() : size(0) {}

template <typename K, Uint64 Capacity>
INLINE bool HashSet<K, Capacity>::Insert(const K& key) {
  Uint64 hash = HashFunction(key) % Capacity;
  if (Contains(key)) return false;
  buckets[hash].PushBack(key);
  size++;
  return true;
}

template <typename K, Uint64 Capacity>
INLINE bool HashSet<K, Capacity>::Remove(const K& key) {
  Uint64 hash = HashFunction(key) % Capacity;
  auto& bucket = buckets[hash];
  for (Uint64 i = 0; i < bucket.Size(); ++i) {
    if (bucket[i] == key) {
      bucket[i] = bucket[bucket.Size() - 1];
      bucket.PopBack();
      size--;
      return true;
    }
  }
  return false;
}

template <typename K, Uint64 Capacity>
INLINE bool HashSet<K, Capacity>::Contains(const K& key) const {
  Uint64 hash = HashFunction(key) % Capacity;
  for (const auto& item : buckets[hash]) {
    if (item == key) return true;
  }
  return false;
}

template <typename K, Uint64 Capacity>
INLINE Uint64 HashSet<K, Capacity>::Size() const {
  return size;
}

template <typename K, Uint64 Capacity>
INLINE bool HashSet<K, Capacity>::IsEmpty() const {
  return size == 0;
}

template <typename K, Uint64 Capacity>
INLINE void HashSet<K, Capacity>::Clear() {
  for (Uint64 i = 0; i < Capacity; ++i) {
    buckets[i].Clear();
  }
  size = 0;
}

template <typename K, Uint64 Capacity>
INLINE K& HashSet<K, Capacity>::operator[](Uint64 index) {
  Uint64 count = 0;
  for (Uint64 i = 0; i < Capacity; ++i) {
    for (Uint64 j = 0; j < buckets[i].Size(); ++j) {
      if (count == index) {
        return buckets[i][j];
      }
      count++;
    }
  }
  throw std::out_of_range("Index out of range");
}

template <typename K, Uint64 Capacity>
INLINE const K& HashSet<K, Capacity>::operator[](Uint64 index) const {
  Uint64 count = 0;
  for (Uint64 i = 0; i < Capacity; ++i) {
    for (Uint64 j = 0; j < buckets[i].Size(); ++j) {
      if (count == index) {
        return buckets[i][j];
      }
      count++;
    }
  }
  throw std::out_of_range("Index out of range");
}

template <typename K, Uint64 Capacity>
INLINE auto HashSet<K, Capacity>::begin() {
  return &buckets[0];
}

template <typename K, Uint64 Capacity>
INLINE auto HashSet<K, Capacity>::end() {
  return &buckets[Capacity];
}

template <typename K, Uint64 Capacity>
INLINE auto HashSet<K, Capacity>::begin() const {
  return &buckets[0];
}

template <typename K, Uint64 Capacity>
INLINE auto HashSet<K, Capacity>::end() const {
  return &buckets[Capacity];
}

template <typename K, Uint64 Capacity>
INLINE Uint64 HashSet<K, Capacity>::HashFunction(const K& key) const {
  return std::hash<K>{}(key);
}

}  // namespace nycatech

#endif  // NYCA_TECH_GAME_HASHSET_H
