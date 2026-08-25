$input a_position, a_texcoord0
$output v_texcoord0

#include "bgfx_shader.sh"

uniform mat4 u_guiProj;
uniform vec4 u_transform;
uniform vec4 u_uv;


void main() {
  vec2 offset = u_transform.xy;
  vec2 size = u_transform.zw;
  vec2 pos = a_position.xy * size + offset;

  //gl_Position = vec4(pos, 0.5, 1.0);
  gl_Position = mul(u_guiProj, vec4(pos, 0.0, 1.0));
  v_texcoord0 = (vec2(1.0, 1.0) - a_position.xy) * u_uv.zw + u_uv.xy;
}