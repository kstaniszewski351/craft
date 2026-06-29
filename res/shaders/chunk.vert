#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec3 iPos;
out vec2 iUV;


layout (std140, binding = 0) uniform CameraData {
  mat4 uView;
  mat4 uProjection;
  vec3 uCameraPos;
};

void main() {

  gl_Position = uProjection * uView * vec4(aPos, 1.0f);

  iPos = aPos;
  iUV = aUV;
}