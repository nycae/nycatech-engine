#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D Texture;
uniform vec3 ViewPos;

struct Light {
  vec3 Position;
  vec3 Color;
  float Intensity;
  float Range;
  int Type;
};

#define MAX_LIGHTS 8
uniform Light Lights[MAX_LIGHTS];
uniform int LightCount;

out vec4 FragColor;

vec3 CalculateLight(Light light, vec3 normal, vec3 fragPos) {
  vec3 lightDir = (light.Type == 0)
    ? normalize(-light.Position)
    : normalize(light.Position - fragPos);
  float diff = dot(normal, lightDir) * 0.5 + 0.5;
  float attenuation = 1.0;
  if (light.Type == 1) {
    float distance = length(light.Position - fragPos);
    attenuation = clamp(1.0 - distance / light.Range, 0.0, 1.0);// Linear falloff
  }
  return diff * light.Color * light.Intensity * attenuation;
//  float attenuation = 1.0;
//  if (light.Type == 1) {
//    float distance = length(light.Position - fragPos);
//    attenuation = 1.0 / (1.0 + 0.1 * distance + 0.01 * distance * distance);
//  }
//
//  vec3 lightDir = normalize(light.Position - fragPos);
//  float diff = max(dot(normal, lightDir), 0.0);
//  vec3 diffuse = diff * light.Color * light.Intensity;
//
//  return diffuse * attenuation;
}

void main() {
  vec3 norm = normalize(Normal);
  //vec3 viewDir = normalize(ViewPos - FragPos);
  vec3 result = vec3(0.4, 0.4, 0.5);

  for (int i = 0; i < LightCount; i++) {
    result += CalculateLight(Lights[i], norm, FragPos);
  }

  vec4 texColor = texture(Texture, TexCoord);
  FragColor = vec4(result, 1.0) * texColor;
}