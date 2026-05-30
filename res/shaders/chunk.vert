#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
// layout (location = 1) in vec3 aNormal;
// layout (location = 2) in vec3 aTangent;
// layout (location = 3) in vec2 aTextureCoord;

out vec3 iPos;
out vec2 iUV;
// out vec3 iNormal;
// out mat3 iTBN;
// out vec2 iTexCoord;

// uniform mat4 uModel;
// uniform mat4 uView;
// uniform mat4 uProjection;

layout (std140, binding = 0) uniform CameraData {
  mat4 uView;
  mat4 uProjection;
  vec3 uCameraPos;
};
// layout (std140, binding = 1) uniform ModelData {
//   ivec2 uPos;
// };

void main() {

  //vec4 world_pos = vec4(uPos.x * 16.0f + aPos.x, uPos.y * 16.0f + aPos.y, aPos.z, 1.0f);

  gl_Position = uProjection * uView * vec4(aPos, 1.0f);

  iPos = aPos;
  iUV = aUV;
  // iTexCoord = aTextureCoord;

  // vec3 normal = normalize(vec3(uModel * vec4(aNormal, 0.0)));
  // vec3 tangent = normalize(vec3(uModel * vec4(aTangent, 0.0)));
  // vec3 bitangent = normalize(vec3(uModel * vec4(cross(aNormal, aTangent), 0.0)));

  // iNormal = normal;

  // iTBN = transpose(mat3(tangent, bitangent, normal));

}