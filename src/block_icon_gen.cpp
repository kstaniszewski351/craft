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

Atlas* GenerateIconAtlas(std::vector<const Block*>& blocks, int icon_size, const Atlas& blocks_atlas) {
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

  bgfx::ProgramHandle shader = Game::Get().GetShaderManager().LoadProgram("atlas_vs.sc", "chunk_fs.sc");

  bgfx::VertexLayout layout;
  layout.begin()
    .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
    .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
    .add(bgfx::Attrib::Color0, 1, bgfx::AttribType::Float)
  .end();

  // bgfx::DynamicVertexBufferHandle vertex_buf = bgfx::createDynamicVertexBuffer((unsigned int)0, layout, BGFX_BUFFER_ALLOW_RESIZE);
  // bgfx::DynamicIndexBufferHandle index_buf = bgfx::createDynamicIndexBuffer((unsigned int)0, BGFX_BUFFER_ALLOW_RESIZE);
  bgfx::UniformHandle s_atlas = bgfx::createUniform("s_atlas", bgfx::UniformType::Sampler);
  


  OrthoCamera camera;
  camera.position = {2, 2, 2};
  camera.rotation = glm::quatLookAt(glm::normalize(- camera.position), DIRECTION_VECTORS[Up]);
  camera.aspect_ratio = 1.0f;
  camera.size = 1.8f;

  std::array<bgfx::TextureHandle, 2> attachments = {color_buf, depth_buf};
  bgfx::FrameBufferHandle framebuffer = bgfx::createFrameBuffer(2, attachments.data());

  bgfx::setViewMode(Views::IconGen, bgfx::ViewMode::Sequential);
  bgfx::setViewFrameBuffer(Views::IconGen, framebuffer);
  bgfx::setViewRect(Views::IconGen, 0, 0, tex_size, tex_size);
  bgfx::setViewClear(
    Views::IconGen,
    BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
    0x00000000
  );


  glm::mat4 view_mat = camera.GetViewMat();
  glm::mat4 proj_mat = camera.GetProjectionMat();

  bgfx::setViewTransform(Views::IconGen, &view_mat, &proj_mat);


  std::vector<ChunkVertex> vertices;
  std::vector<uint16_t> triangles;
  std::vector<unsigned int> vertex_offsets(blocks.size());
  std::vector<unsigned int> vertex_lengths(blocks.size());
  std::vector<unsigned int> triangle_offsets(blocks.size());
  std::vector<unsigned int> triangle_lengths(blocks.size());
  for(int i = 0; i < blocks.size(); i++) {

    int block_id = blocks[i]->GetRegistryID();
    
    vertex_offsets[i] = vertices.size();
    triangle_offsets[i] = triangles.size();
    for(int d = 0; d < DirectionCount; d++) {
      addBlockFace((Direction)d, {0, 0, 0}, block_id, 15, blocks_atlas, vertices, triangles);
    }
    vertex_lengths[i] = vertices.size() - vertex_offsets[i];
    triangle_lengths[i] = triangles.size() - triangle_offsets[i];
  }
  bgfx::VertexBufferHandle vertex_buf = bgfx::createVertexBuffer(bgfx::copy(vertices.data(), vertices.size() * sizeof(ChunkVertex)), layout);
  bgfx::IndexBufferHandle index_buf = bgfx::createIndexBuffer(bgfx::copy(triangles.data(), triangles.size() * sizeof(uint16_t)));

  bgfx::UniformHandle u_scale_pos = bgfx::createUniform("u_scalePos", bgfx::UniformType::Vec4);
  glm::vec2 scale = glm::vec2(1.0f / pow2_size);

  for(int i = 0; i < blocks.size(); i++) {
    bgfx::setState(BGFX_STATE_DEFAULT);

    int x = (i % pow2_size);
    int y = (i / pow2_size);
    glm::vec2 pos = glm::vec2((float)x / pow2_size, (float)y / pow2_size);
    glm::vec4 scale_pos = glm::vec4(scale, pos);
    bgfx::setUniform(u_scale_pos, &scale_pos);

    bgfx::setTexture(0, s_atlas, blocks_atlas.getTexture());
    bgfx::setVertexBuffer(0, vertex_buf);
    bgfx::setIndexBuffer(index_buf, triangle_offsets[i], triangle_lengths[i]);

    bgfx::submit(Views::IconGen, shader);
  }

  bgfx::destroy(u_scale_pos);
  bgfx::destroy(framebuffer);
  bgfx::destroy(depth_buf);
  bgfx::destroy(shader);
  bgfx::destroy(index_buf);
  bgfx::destroy(vertex_buf);
  bgfx::destroy(s_atlas);

  return new Atlas(color_buf, pow2_size, icon_size);
}