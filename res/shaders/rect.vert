#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

layout (location = 0) uniform mat4 uProjection;
layout (location = 1) uniform vec2 uPos;
layout (location = 2) uniform vec2 uScale;

out vec2 uv;

void main() {
  gl_Position = uProjection * vec4(aPos * uScale + uPos, 0.0, 1.0);
  uv = aUV;
}