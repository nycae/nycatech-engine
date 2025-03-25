//
// Created by rplaz on 2025-03-15.
//

#ifndef NYCATECH_ENGINE_SHADER_H
#define NYCATECH_ENGINE_SHADER_H

#include <glad/glad.h>

#include "ecs.h"

namespace nycatech {

struct Shader {
  enum Type : Uint32 {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
  };

  Type   type;
  String source;
  Uint64 id = 0;
};

struct ShaderProgram : public Component {
  Uint64                   id;
  Vector<SmartPtr<Shader>> shaders;
};

class ShaderFactory {
public:
  class ShaderBuilder {
  public:
    ShaderBuilder&   from_file(const String& path);
    ShaderBuilder&   from_string(const String& content);
    ShaderBuilder&   with_type(Shader::Type type);
    ShaderBuilder&   with_name(const String& name);
    SmartPtr<Shader> build();

  private:
    String           name;
    SmartPtr<Shader> inner = make_shared<Shader>();
  };

  class ProgramBuilder {
  public:
    ProgramBuilder&         with_shader(SmartPtr<Shader> shader);
    ProgramBuilder&         with_name(const String& name);
    SmartPtr<ShaderProgram> build();

  public:
    String                  name;
    SmartPtr<ShaderProgram> inner = make_shared<ShaderProgram>();
  };

private:
  ShaderFactory() = default;

public:
  static ProgramBuilder create_program();
  static ShaderBuilder  create_shader();
};

}  // namespace nycatech

#endif  // NYCATECH_ENGINE_SHADER_H
