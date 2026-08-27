$input v_texcoord0, v_brightness

#include "bgfx_shader.sh"

SAMPLER2D(s_atlas, 0);

void main() {
  vec4 tex = texture2D(s_atlas, v_texcoord0);
  vec3 color = tex.rgb;
  color *= (v_brightness + 0.05);
  gl_FragColor = vec4(color, tex.a);
}