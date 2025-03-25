//
// Created by rplaz on 2025-03-15.
//

#include "shader.h"

namespace nycatech {

ShaderFactory::ShaderBuilder& ShaderFactory::ShaderBuilder::from_file(const String& path)
{
  FileReader file(path.c_str());
  String     file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  return from_string(file_content);
}

ShaderFactory::ShaderBuilder& ShaderFactory::ShaderBuilder::from_string(const String& content)
{
  inner->source = content;
  return Self;
}

SmartPtr<Shader> ShaderFactory::ShaderBuilder::build()
{
  const char* source = inner->source.c_str();
  inner->id = glCreateShader(inner->type);
  glShaderSource(inner->id, 1, &source, nullptr);
  glCompileShader(inner->id);
  GLint success;
  glGetShaderiv(inner->id, GL_COMPILE_STATUS, &success);
  inner->source.clear();
  return success ? inner : nullptr;
}

ShaderFactory::ShaderBuilder& ShaderFactory::ShaderBuilder::with_type(Shader::Type type)
{
  inner->type = type;
  return Self;
}

ShaderFactory::ShaderBuilder& ShaderFactory::ShaderBuilder::with_name(const String& new_name)
{
  Self.name = new_name;
  return Self;
}

ShaderFactory::ProgramBuilder ShaderFactory::create_program()
{
  return ShaderFactory::ProgramBuilder();
}

ShaderFactory::ShaderBuilder ShaderFactory::create_shader()
{
  return ShaderFactory::ShaderBuilder();
}

SmartPtr<ShaderProgram> ShaderFactory::ProgramBuilder::build()
{
  inner->id = glCreateProgram();
  for (const auto &shader : inner->shaders) {
    glAttachShader(inner->id, shader->id);
  }
  glLinkProgram(inner->id);
  GLint success;
  glGetProgramiv(inner->id, GL_LINK_STATUS, &success);
  return success ? inner : nullptr;
}

ShaderFactory::ProgramBuilder& ShaderFactory::ProgramBuilder::with_shader(SmartPtr<Shader> shader)
{
  inner->shaders.push_back(shader);
  return Self;
}

ShaderFactory::ProgramBuilder& ShaderFactory::ProgramBuilder::with_name(const String& new_name)
{
  Self.name = new_name;
  return Self;
}

}  // namespace nycatech