//
// Created by rplaz on 2025-03-15.
//

#ifndef NYCATECH_ENGINE_SHADER_H
#define NYCATECH_ENGINE_SHADER_H

#include "ecs.h"

#include <glad/glad.h>

namespace nycatech {

class Shader {
 public:
  enum Type : Uint32 {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
  };

  Type type;
  String source;
  Uint64 id = 0;

  Shader(Type type = Type::Vertex) : type(type) {}
};

class ShaderProgram {
 public:
  Uint64 id;
  Vector<SmartPtr<Shader>> shaders;
};

struct ShaderComponent : public Component {
  String name;
  ShaderComponent(String name) : name(move(name)) {}
};

class ShaderFactory {
 public:
  static ShaderFactory& instance();

  class ShaderBuilder {
   public:
    ShaderBuilder() : inner(make_shared<Shader>()) {}
    ShaderBuilder& from_file(const String& path);
    ShaderBuilder& from_string(String content);
    ShaderBuilder& with_type(Shader::Type type);
    ShaderBuilder& with_name(String);
    SmartPtr<Shader> build();

   private:
    String name;
    SmartPtr<Shader> inner;
  };

  class ProgramBuilder {
   public:
    ProgramBuilder() : inner(make_shared<ShaderProgram>()) {}
    ProgramBuilder& with_shader(SmartPtr<Shader> shader);
    ProgramBuilder& with_name(String name);
    SmartPtr<ShaderProgram> build();

   public:
    String name;
    SmartPtr<ShaderProgram> inner;
  };

 private:
  ShaderFactory() = default;

 public:
  ProgramBuilder create_program();
  ShaderBuilder create_shader();
  SmartPtr<ShaderProgram> get(const String& name);

 public:
  Map<String, SmartPtr<Shader>> shaders;
  Map<String, SmartPtr<ShaderProgram>> shader_programs;
};

}  // namespace nycatech

#endif  // NYCATECH_ENGINE_SHADER_H
