//
// Created by rplaz on 2025-02-21.
//

#include "package.h"

#include <fstream>
#include <sstream>

namespace nycatech {

void Package::add_asset(const String& file) { assets.push_back(file); }

void Package::bundle(const String& file) {
  FileWritter out_file(file.c_str(), std::ios::binary);
  ZSTD_CCtx* ctx = ZSTD_createCCtx();

  Vector<char> out_buff;
  out_buff.resize(ZSTD_compressBound(size_buffer));

  for (const auto& asset_file : assets) {
    FileReader in_file(asset_file.c_str(), std::ios::binary);
    StringBuilder buffer;
    buffer << in_file.rdbuf();
    String data = buffer.str();
    in_file.close();
    Uint64 size_compressed
        = ZSTD_compressCCtx(ctx, out_buff.data(), out_buff.capacity(), data.data(), data.size(), ZSTD_defaultCLevel());
    out_file.write(reinterpret_cast<const char*>(&size_compressed), sizeof(size_compressed));
    out_file.write(out_buff.data(), size_compressed);
  }

  out_file.close();
  ZSTD_freeCCtx(ctx);
}

Uint64 Package::size_of_files() {
  Uint64 size = 0;
  for (const auto& asset_file : assets) {
    FILE* const input_file = fopen(asset_file.c_str(), "rb");
    fseek(input_file, 0, SEEK_END);
    size += ftell(input_file);
    fclose(input_file);
  }

  return size;
}

void Package::load_from(const String& file) {
  FileReader in_file(file.c_str(), std::ios::binary);
  ZSTD_DCtx* const ctx = ZSTD_createDCtx();
  Vector<char> out_buffer;
  out_buffer.resize(size_buffer);

  while (in_file) {
    Uint64 size_compressed;
    in_file.read(reinterpret_cast<char*>(&size_compressed), sizeof(size_compressed));
    if (in_file.gcount() == 0) break;
    Vector<char> in_buffer;
    in_buffer.resize(size_compressed);
    in_file.read(in_buffer.data(), size_compressed);
    Uint64 size_decompressed
        = ZSTD_decompressDCtx(ctx, out_buffer.data(), out_buffer.capacity(), in_buffer.data(), size_compressed);
    String s(out_buffer.data(), size_decompressed);
    asset_content.push_back(s);
  }

  in_file.close();
  ZSTD_freeDCtx(ctx);
}

}  // namespace nycatech