//
// Created by rplaz on 2025-02-21.
//

#ifndef NYCA_TECH_PACKAGE_H
#define NYCA_TECH_PACKAGE_H

#include <zstd.h>

#include <istream>

#include "base.h"

namespace nycatech {

class Package {
 public:
  Package() = default;
  ~Package() = default;

  void add_asset(const String& file);
  void bundle(const String& file);

  void load_from(const String& file);

 public:
  Vector<String> assets;
  Vector<String> asset_content;

 private:
  static const Uint64 size_buffer = 1 << 24;  // 16Mbytes
  Uint64 size_of_files();
};

}  // namespace nycatech

#endif  // NYCA_TECH_PACKAGE_H
