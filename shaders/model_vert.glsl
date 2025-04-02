#version 330 core
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec3 InNormal;
layout(location = 2) in vec2 InTexCoord;

uniform mat4 CameraProjection;
uniform mat4 CameraTransform;
uniform mat4 Transform;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
  gl_Position = CameraProjection * CameraTransform * Transform * vec4(InPosition, 1.0f);
  Position = vec3(Transform * vec4(InPosition, 1.0));
  Normal = mat3(transpose(inverse(Transform))) * InNormal;
  TexCoord = InTexCoord;
}