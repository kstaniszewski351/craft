#include "block_icon_gen.h"
#include "atlas.h"
#include "bgfx/bgfx.h"
#include "bgfx/defines.h"
#include "camera_data.h"
#include "chunk_mesh.h"
#include "chunk_vertex.h"
#include "direction.h"
#include "ortho_camera.h"
#include "views.h"
#include "world_renderer.h"
#include <cstdint>
#include <glm/gtc/quaternion.hpp>
#include "game.h"

Atlas GenerateIconAtlas(std::vector<int>& blocks, int icon_size, const Atlas& blocks_atlas) {
  int pow2_size = findSize(blocks.size());
  int tex_size = pow2_size * icon_size;

  bgfx::TextureHandle depth_buf = bgfx::createTexture2D(
    tex_size,
    tex_size,
    false,
    1,
    bgfx::TextureFormat::D24,
    BGFX_TEXTURE_RT
  );
  bgfx::TextureHandle color_buf = bgfx::createTexture2D(
    tex_size,
    tex_size,
    false,
    1,
    bgfx::TextureFormat::RGBA8,
    BGFX_TEXTURE_RT
  );

  bgfx::ProgramHandle shader = Game::Get().GetShaderManager().LoadProgram("chunk_vs.sc", "chunk_fs.sc");

  bgfx::VertexLayout layout;
  layout.begin()
    .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
    .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
  .end();

  bgfx::DynamicVertexBufferHandle vertex_buf = bgfx::createDynamicVertexBuffer((unsigned int)0, layout, BGFX_BUFFER_ALLOW_RESIZE);
  bgfx::DynamicIndexBufferHandle index_buf = bgfx::createDynamicIndexBuffer((unsigned int)0, BGFX_BUFFER_ALLOW_RESIZE);
  bgfx::UniformHandle s_atlas = bgfx::createUniform("s_atlas", bgfx::UniformType::Sampler);
  


  OrthoCamera camera;
  camera.position = {2, 2, 2};
  camera.rotation = glm::quatLookAt(glm::normalize(- camera.position), DIRECTION_VECTORS[Up]);
  camera.aspect_ratio = -1.0f;
  camera.size = 1.8f;

  std::array<bgfx::TextureHandle, 2> attachments = {color_buf, depth_buf};
  bgfx::FrameBufferHandle framebuffer = bgfx::createFrameBuffer(2, attachments.data());

  bgfx::setViewFrameBuffer(Views::IconGen, framebuffer);
  bgfx::setViewRect(Views::IconGen, 0, 0, tex_size, tex_size);
  bgfx::setViewClear(
    Views::IconGen,
    BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
  );
  bgfx::setState(BGFX_STATE_DEFAULT);

  bgfx::setVertexBuffer(0, vertex_buf);
  bgfx::setIndexBuffer(index_buf);
  
  bgfx::setTexture(0, s_atlas, blocks_atlas.getTexture());

  glm::mat4 view_mat = camera.GetViewMat();
  glm::mat4 proj_mat = camera.GetProjectionMat();

  bgfx::setViewTransform(Views::IconGen, &view_mat, &proj_mat);



  for(int i = 0; i < blocks.size(); i++) {
    std::vector<ChunkVertex> vertices;
    std::vector<uint16_t> triangles;
  
    for(int d = 0; d < DirectionCount; d++) {
      addBlockFace((Direction)d, {0, 0, 0}, blocks[i], blocks_atlas, vertices, triangles);
    }
    bgfx::update(vertex_buf, 0, bgfx::copy(vertices.data(), vertices.size() * sizeof(ChunkVertex)));
    bgfx::update(index_buf, 0, bgfx::copy(triangles.data(), triangles.size() * sizeof(uint16_t)));

    int x = (i % pow2_size) * icon_size;
    int y = (i / pow2_size) * icon_size;

    bgfx::setViewRect(Views::IconGen, x, y, icon_size, icon_size);

    bgfx::submit(Views::IconGen, shader);
  }

  bgfx::destroy(depth_buf);
  bgfx::destroy(shader);
  bgfx::destroy(index_buf);
  bgfx::destroy(vertex_buf);
  bgfx::destroy(s_atlas);

  return Atlas(color_buf, pow2_size, icon_size);
}