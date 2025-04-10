//
// Created by rplaz on 2025-03-15.
//

#include "shader.h"

#include "base.h"

namespace nycatech {

ShaderFactory::ShaderBuilder& ShaderFactory::ShaderBuilder::FromFile(const String& Path)
{
  FileReader File(Path);
  String     FileContent((std::istreambuf_iterator<char>(File)), std::istreambuf_iterator<char>());
  return FromString(FileContent);
}

ShaderFactory::ShaderBuilder& ShaderFactory::ShaderBuilder::FromString(const String& Content)
{
  Source = Content;
  return Self;
}

Shader ShaderFactory::ShaderBuilder::Build()
{
  Shader      Shader{ .Id = glCreateShader(ShaderType) };
  const char* ShaderSource = Source.c_str();
  glShaderSource(Shader.Id, 1, &ShaderSource, nullptr);
  glCompileShader(Shader.Id);
#ifdef DEBUG
  GLint Success;
  glGetShaderiv(Shader.Id, GL_COMPILE_STATUS, &Success);
  if (!Success) {
    Int32 MaxLength;
    glGetShaderiv(Shader.Id, GL_COMPILE_STATUS, &MaxLength);
    String Error(MaxLength, ' ');
    glGetShaderInfoLog(Shader.Id, MaxLength, &MaxLength, Error.data());
    assert(Success && Error.c_str());
  }
#endif
  return Shader;
}

ShaderFactory::ShaderBuilder& ShaderFactory::ShaderBuilder::WithType(Uint32 Type)
{
  ShaderType = Type;
  return Self;
}

ShaderFactory::ProgramBuilder ShaderFactory::CreateProgram()
{
  return ShaderFactory::ProgramBuilder();
}

ShaderFactory::ShaderBuilder ShaderFactory::CreateShader()
{
  return ShaderFactory::ShaderBuilder();
}

ShaderProgram ShaderFactory::ProgramBuilder::Build()
{
  ShaderProgram Program{ .Id = glCreateProgram() };
  for (const auto& Shader : Shaders) {
    glAttachShader(Program.Id, Shader.Id);
  }
  glLinkProgram(Program.Id);
#ifdef DEBUG
  GLint Success;
  glGetProgramiv(Program.Id, GL_LINK_STATUS, &Success);
  assert(Success);
#endif
  return Program;
}

ShaderFactory::ProgramBuilder& ShaderFactory::ProgramBuilder::WithShader(Shader Shader)
{
  Shaders.push_back(std::move(Shader));
  return Self;
}

}  // namespace nycatech