#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec4 aColor;

layout (location = 0) uniform mat4 uProjection;

out vec2 uv;
out vec4 color;

void main() {
  gl_Position = uProjection * vec4(aPos, 0.0f, 1.0);
  uv = aUV;
  color = aColor;
}