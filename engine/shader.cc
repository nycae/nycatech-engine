//
// Created by rplaz on 2025-03-15.
//

#include "shader.h"

namespace nycatech {

ShaderFactory& ShaderFactory::instance() {
  static ShaderFactory instance;
  return instance;
}

ShaderFactory::ShaderBuilder& ShaderFactory::ShaderBuilder::from_file(const String& path) {
  FileReader file(path);
  String file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  return from_string(file_content);
}

ShaderFactory::ShaderBuilder& ShaderFactory::ShaderBuilder::from_string(String content) {
  inner->source = move(content);
  return Self;
}

SmartPtr<Shader> ShaderFactory::ShaderBuilder::build() {
  ShaderFactory::instance().shaders[name] = inner;
  return inner;
}

ShaderFactory::ShaderBuilder& ShaderFactory::ShaderBuilder::with_type(Shader::Type type) {
  inner->type = type;
  return Self;
}

ShaderFactory::ShaderBuilder& ShaderFactory::ShaderBuilder::with_name(String name) {
  Self.name = move(name);
  return Self;
}

SmartPtr<ShaderProgram> ShaderFactory::get(const String& name) {
  return shader_programs[name];
}

ShaderFactory::ProgramBuilder ShaderFactory::create_program() {
  return ShaderFactory::ProgramBuilder();
}
ShaderFactory::ShaderBuilder ShaderFactory::create_shader() {
  return ShaderFactory::ShaderBuilder();
}

SmartPtr<ShaderProgram> ShaderFactory::ProgramBuilder::build() {
  ShaderFactory::instance().shader_programs[name] = inner;
  return inner;
}

ShaderFactory::ProgramBuilder& ShaderFactory::ProgramBuilder::with_shader(SmartPtr<Shader> shader) {
  inner->shaders.push_back(move(shader));
  return Self;
}

ShaderFactory::ProgramBuilder& ShaderFactory::ProgramBuilder::with_name(String name) {
  Self.name = move(name);
  return Self;
}

}  // namespace nycatech