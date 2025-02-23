//
// Created by rplaz on 2025-02-21.
//

#include "package.h"

#include <fstream>
#include <sstream>

namespace nycatech {

void Package::AddAsset(const String& file) { assets.PushBack(file); }

void Package::Bundle(const String& file) {
  FileWritter outFile(file.c_str(), std::ios::binary);
  ZSTD_CCtx* ctx = ZSTD_createCCtx();

  Vector<char> outBuffer;
  outBuffer.GrowToSize(ZSTD_compressBound(SizeBuffer));

  for (const auto& assetFile : assets) {
    FileReader inFile(assetFile.c_str(), std::ios::binary);
    StringBuilder buffer;
    buffer << inFile.rdbuf();
    String data = buffer.str();
    inFile.close();
    Uint64 sizeCompressed = ZSTD_compressCCtx(
        ctx, outBuffer.Data(), outBuffer.Allocated(), data.data(), data.size(), ZSTD_defaultCLevel());
    outFile.write(reinterpret_cast<const char*>(&sizeCompressed), sizeof(sizeCompressed));
    outFile.write(outBuffer.Data(), sizeCompressed);
  }

  outFile.close();
  ZSTD_freeCCtx(ctx);
}

Uint64 Package::SizeOfFiles() {
  Uint64 size = 0;
  for (const auto& assetFile : assets) {
    FILE* const inputFile = fopen(assetFile.c_str(), "rb");
    fseek(inputFile, 0, SEEK_END);
    size += ftell(inputFile);
    fclose(inputFile);
  }

  return size;
}

void Package::LoadFrom(const String& file) {
  FileReader inFile(file.c_str(), std::ios::binary);
  ZSTD_DCtx* const ctx = ZSTD_createDCtx();
  Vector<char> outBuffer;
  outBuffer.GrowToSize(SizeBuffer);

  while (inFile) {
    Uint64 sizeCompressed;
    inFile.read(reinterpret_cast<char*>(&sizeCompressed), sizeof(sizeCompressed));
    if (inFile.gcount() == 0) break;
    Vector<char> inBuffer;
    inBuffer.GrowToSize(sizeCompressed);
    inFile.read(inBuffer.Data(), sizeCompressed);
    Uint64 sizeDecompressed =
        ZSTD_decompressDCtx(ctx, outBuffer.Data(), outBuffer.Allocated(), inBuffer.Data(), sizeCompressed);
    String s(outBuffer.Data(), sizeDecompressed);
    assetContent.PushBack(s);
  }

  inFile.close();
  ZSTD_freeDCtx(ctx);
}

}  // namespace nycatech