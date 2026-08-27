$input a_position, a_texcoord0, a_color0
$output v_texcoord0, v_brightness

#include "bgfx_shader.sh"

void main() {

  gl_Position = mul(u_viewProj, vec4(a_position, 1.0f));

  v_texcoord0 = a_texcoord0;
  v_brightness = a_color0;
}