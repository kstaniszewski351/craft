#version 460 core

out vec4 FragColor;

in vec3 iPos;
in vec2 iUV;

layout(binding = 0) uniform sampler2D uAtlas;


void main() {
  //FragColor = vec4(mod(iPos.x, 1.0), mod(iPos.y, 1.0), mod(iPos.z, 1.0), 1.0);
  FragColor = texture(uAtlas, iUV);
  //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}