#version 460 core

out vec4 FragColor;

in vec3 iPos;
in vec2 iUV;

layout(binding = 0) uniform sampler2D uAtlas;


void main() {
  FragColor = texture(uAtlas, iUV);
}