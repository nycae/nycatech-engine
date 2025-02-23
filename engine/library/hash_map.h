#ifndef NYCA_TECH_HASHMAP_H
#define NYCA_TECH_HASHMAP_H

#include <functional>
#include <utility>

#include "base.h"
#include "vector.h"

namespace nycatech {

constexpr Uint64 DEFAULT_CAPACITY = 16;

template <typename K, typename V, Uint64 Capacity = DEFAULT_CAPACITY>
class HashMap {
 public:
  INLINE HashMap();
  INLINE ~HashMap() = default;

  INLINE bool Insert(const K& key, const V& value);
  INLINE bool At(const K& key, V* value) const;
  INLINE bool Remove(const K& key);
  INLINE bool Contains(const K& key) const;
  INLINE Uint64 Size() const;
  INLINE bool IsEmpty() const;
  INLINE void Clear();

  INLINE Pair<K, V>* begin() { return &buckets[0]; }
  INLINE Pair<K, V>* end() { return &buckets[Capacity]; }
  INLINE const Pair<K, V>* begin() const { return &buckets[0]; }
  INLINE const Pair<K, V>* end() const { return &buckets[Capacity]; }

  INLINE V& operator[](const K& key);
  INLINE const V& operator[](const K& key) const;

  INLINE HashMap& operator=(HashMap&& other);

 private:
  INLINE Uint64 HashFunction(const K& key) const;

 private:
  Vector<Pair<K, V>> buckets[Capacity];
  Uint64 size;
};

template <typename K, typename V, Uint64 Capacity>
HashMap<K, V, Capacity>& HashMap<K, V, Capacity>::operator=(HashMap&& other) {
  Self.buckets = other.buckets;
  Self.size = other.size;
  delete other;
  return Self;
}

template <typename K, typename V, Uint64 Capacity>
INLINE HashMap<K, V, Capacity>::HashMap() : size(0) {}

template <typename K, typename V, Uint64 Capacity>
INLINE bool HashMap<K, V, Capacity>::Insert(const K& key, const V& value) {
  Uint64 hash = HashFunction(key) % Capacity;
  for (auto& pair : buckets[hash]) {
    if (pair.first == key) {
      pair.second = value;
      return true;
    }
  }
  buckets[hash].Insert(Pair(key, value));
  size++;
  return true;
}

template <typename K, typename V, Uint64 Capacity>
INLINE bool HashMap<K, V, Capacity>::At(const K& key, V* value) const {
  Uint64 hash = HashFunction(key) % Capacity;
  for (const auto& pair : buckets[hash]) {
    if (pair.first == key) {
      *value = pair.second;
      return true;
    }
  }
  return false;
}

template <typename K, typename V, Uint64 Capacity>
INLINE bool HashMap<K, V, Capacity>::Remove(const K& key) {
  Uint64 hash = HashFunction(key) % Capacity;
  auto& bucket = buckets[hash];
  for (Uint64 i = 0; i < bucket.Count(); ++i) {
    if (bucket[i].first != key) continue;
    bucket[i] = bucket[bucket.Count() - 1];
    bucket.OverrideCount(bucket.Count() - 1);
    size--;
    return true;
  }
  return false;
}

template <typename K, typename V, Uint64 Capacity>
INLINE bool HashMap<K, V, Capacity>::Contains(const K& key) const {
  V value;
  return At(key, &value);
}

template <typename K, typename V, Uint64 Capacity>
INLINE Uint64 HashMap<K, V, Capacity>::Size() const {
  return size;
}

template <typename K, typename V, Uint64 Capacity>
INLINE bool HashMap<K, V, Capacity>::IsEmpty() const {
  return size == 0;
}

template <typename K, typename V, Uint64 Capacity>
INLINE void HashMap<K, V, Capacity>::Clear() {
  for (Uint64 i = 0; i < Capacity; ++i) {
    buckets[i].OverrideCount(0);
  }
  size = 0;
}

template <typename K, typename V, Uint64 Capacity>
INLINE Uint64 HashMap<K, V, Capacity>::HashFunction(const K& key) const {
  return std::hash<K>{}(key);
}

template <typename K, typename V, Uint64 Capacity>
INLINE V& HashMap<K, V, Capacity>::operator[](const K& key) {
  Uint64 hash = HashFunction(key) % Capacity;
  for (auto& pair : buckets[hash]) {
    if (pair.first == key) {
      return pair.second;
    }
  }
  buckets[hash].Insert(Pair(key, V()));
  size++;
  return buckets[hash].Data()[buckets[hash].Count() - 1].second;
}

template <typename K, typename V, Uint64 Capacity>
const V& HashMap<K, V, Capacity>::operator[](const K& key) const {
  Uint64 hash = HashFunction(key) % Capacity;
  for (auto& pair : buckets[hash]) {
    if (pair.first == key) {
      return pair.second;
    }
  }
  return V();
}

}  // namespace nycatech

#endif  // NYCA_TECH_HASHMAP_H
