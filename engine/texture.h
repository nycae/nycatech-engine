//
// Created by rplaz on 2025-03-24.
//

#ifndef NYCATECH_ENGINE_TEXTURE_H
#define NYCATECH_ENGINE_TEXTURE_H

#include "base.h"
#include "ecs.h"

namespace nycatech {

struct Texture : public Component {
  Uint32 id;

  static SmartPtr<Texture> from_file(const String& file_path);
};


}  // namespace nycatech

#endif  // NYCATECH_ENGINE_TEXTURE_H
