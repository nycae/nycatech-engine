//
// Created by rplaz on 2025-02-21.
//

#ifndef NYCA_TECH_PACKAGE_H
#define NYCA_TECH_PACKAGE_H

#include <istream>

#include "base.h"
#include "vector.h"
#include "zstd.h"

namespace nycatech {

class Package {
 public:
  Package() = default;
  ~Package() = default;

  void AddAsset(const String& file);
  void Bundle(const String& file);

  void LoadFrom(const String& file);

 public:
  Vector<String> assets;
  Vector<String> assetContent;

 private:
  static const Uint64 SizeBuffer = 1 << 24; // 16Mbytes
  Uint64 SizeOfFiles();
};

}  // namespace nycatech

#endif  // NYCA_TECH_PACKAGE_H
