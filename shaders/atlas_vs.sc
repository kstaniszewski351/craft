$input a_position, a_texcoord0
$output v_texcoord0

#include "bgfx_shader.sh"

uniform vec4 u_scalePos;

void main() {

  gl_Position = mul(u_viewProj, vec4(a_position, 1.0f));
  gl_Position += vec4(1.0, 1.0, 0.0, 0.0);
  gl_Position *= vec4(u_scalePos.xy, 1.0, 1.0);
  gl_Position -= vec4(1.0, 1.0, 0.0, 0.0);
  gl_Position += vec4(u_scalePos.zw, 0.0, 0.0) * 2.0;

  v_texcoord0 = a_texcoord0;
}