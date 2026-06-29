#include "block_icon_gen.h"
#include "atlas.h"
#include "camera_data.h"
#include "chunk_mesh.h"
#include "chunk_vertex.h"
#include "direction.h"
#include "gfx/framebuffer.h"
#include "glad/gl.h"
#include "ortho_camera.h"
#include "gfx/shader.h"
#include "world_renderer.h"
#include <glm/gtc/quaternion.hpp>

Atlas GenerateIconAtlas(std::vector<int>& blocks, int icon_size, const Atlas& blocks_atlas) {
  int pow2_size = findSize(blocks.size());
  int tex_size = pow2_size * icon_size;

  GFX::Texture depth_buf(tex_size, tex_size, GL_DEPTH_COMPONENT24);
  GFX::Texture color_buf(tex_size, tex_size, GL_RGBA8);
  GFX::Shader shader("res/shaders/chunk.frag", "res/shaders/chunk.vert");
  GFX::VAO vao(CHUNK_VERTEX_FORMAT.begin(), CHUNK_VERTEX_FORMAT.end(), sizeof(ChunkVertex));
  GFX::Framebuffer fbo;
  GFX::Buffer camera_buf;
  GFX::Buffer vbo;
  GFX::Buffer ebo;
  
  shader.BindUniformBlock(0, 0);

  fbo.AttachTexture(GL_COLOR_ATTACHMENT0, color_buf);
  fbo.AttachTexture(GL_DEPTH_ATTACHMENT, depth_buf);
  OrthoCamera camera;
  camera.position = {2, 2, 2};
  camera.rotation = glm::quatLookAt(glm::normalize(- camera.position), direction_vectors[Up]);
  camera.aspect_ratio = -1.0f;
  camera.size = 1.8f;
  CameraData data {
    camera.GetViewMat(),
    camera.GetProjectionMat(),
    camera.position
  };


  vao.Bind();
  vbo.BindVertexBuffer(0, 0, sizeof(ChunkVertex));
  ebo.Bind(GL_ELEMENT_ARRAY_BUFFER);
  shader.Use();
  blocks_atlas.getTexture().Bind(0);

  camera_buf.BindTarget(GL_UNIFORM_BUFFER, 0);
  camera_buf.Data(sizeof(CameraData), &data);


  fbo.Bind(GL_FRAMEBUFFER);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for(int i = 0; i < blocks.size(); i++) {
    std::vector<ChunkVertex> vertices;
    std::vector<unsigned int> triangles;
  
    for(int d = 0; d < DirectionCount; d++) {
      addBlockFace((Direction)d, {0, 0, 0}, blocks[i], blocks_atlas, vertices, triangles);
    }
    vbo.Data(sizeof(ChunkVertex) * vertices.size(), vertices.data());
    ebo.Data(sizeof(unsigned int) * triangles.size(), triangles.data());

    int x = (i % pow2_size) * icon_size;
    int y = (i / pow2_size) * icon_size;

    glViewport(x, y, icon_size, icon_size);


    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, 0);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return Atlas(std::move(color_buf), pow2_size, icon_size);
}