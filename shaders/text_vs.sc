$input a_position, a_texcoord0
$output v_texcoord0

#include "bgfx_shader.sh"
uniform mat4 u_guiProj;

void main() {
  gl_Position = mul(u_guiProj, vec4(a_position, 1.0f));
  v_texcoord0 = a_texcoord0;
}