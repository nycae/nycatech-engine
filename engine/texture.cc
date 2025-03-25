//
// Created by rplaz on 2025-03-24.
//

#include "texture.h"

#include <glad/glad.h>

#include "stb_image.h"

namespace nycatech {


SmartPtr<Texture> Texture::from_file(const String& file_path) {
  Int32 width, height, channels;
  auto texture = make_shared<Texture>();
  auto data = stbi_load(file_path.c_str(), &width, &height, &channels, 0);

  glGenTextures(1, &texture->id);
  glBindTexture(GL_TEXTURE_2D, texture->id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
  return texture;
}

}  // namespace nycatech