#version 330 core
in vec3 Normal;
in vec2 Uv;

uniform sampler2D Texture;

out vec4 FragColor;

void main()
{
  FragColor = texture(Texture, Uv);
}