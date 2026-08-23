#version 460 core

in vec2 uv;
in vec4 color;

layout (binding = 0) uniform sampler2D uTexture;

out vec4 out_Color;

void main() {
  out_Color = color * texture(uTexture, uv);
}