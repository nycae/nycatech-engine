//
// Created by rplaz on 2025-03-15.
//

#pragma once

#include <glad/glad.h>

#include "base.h"

namespace nycatech {

struct Shader {
  Uint64 Id = 0;
  Uint32 ShaderType = 0;
};

struct ShaderProgram {
  Uint64 Id;
};

class ShaderFactory {
public:
  class ShaderBuilder {
  public:
    ShaderBuilder& FromFile(const String& Path);
    ShaderBuilder& FromString(const String& Content);
    ShaderBuilder& WithType(Uint32 Type);
    Shader         Build();

  private:
    Uint32 ShaderType;
    String Source;
  };

  class ProgramBuilder {
  public:
    ProgramBuilder& WithShader(Shader Shader);
    ShaderProgram   Build();

  public:
    Vector<Shader> Shaders;
  };

private:
  ShaderFactory() = default;

public:
  static ProgramBuilder CreateProgram();
  static ShaderBuilder  CreateShader();
};

}  // namespace nycatech
