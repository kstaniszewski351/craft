$input a_position, a_texcoord0
$output v_texcoord0

#include "bgfx_shader.sh"

uniform vec4 u_transform;
uniform vec4 u_uv;


void main() {
  vec2 offset = u_transform.xy;
  vec2 size = u_transform.zw;
  vec2 pos = a_position.xy * size + offset;

  gl_Position = mul(u_viewProj, vec4(pos, 1.0, 1.0));
  v_texcoord0 = a_position.xy * u_uv.zw + u_uv.xy;
}