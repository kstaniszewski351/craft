#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 iPos;

layout (std140, binding = 0) uniform CameraData {
  mat4 uView;
  mat4 uProjection;
  vec3 uCameraPos;
};

layout(binding = 1) uniform ModelBlock {
    mat4 uModel;
};

void main() {
  
  //vec3 worldPos = uModelScale * uModelPos * aPos;

  gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0f);
  iPos = aPos;

}