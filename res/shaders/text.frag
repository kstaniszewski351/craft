#version 460 core

in vec2 uv;

layout (binding = 0) uniform sampler2D uTexture;

out vec4 out_Color;

void main() {
  float alpha = texture(uTexture, uv).r;
  out_Color = vec4(1.0, 1.0, 1.0, alpha);
}