#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec2 InUv;

uniform mat4 CameraProjection;
uniform mat4 CameraTransform;
uniform mat4 Transform;

out vec3 Normal;
out vec2 Uv;

void main()
{
  gl_Position = CameraProjection * CameraTransform * Transform * vec4(Position, 1.0f);
  Normal = InNormal;
  Uv = InUv;
}