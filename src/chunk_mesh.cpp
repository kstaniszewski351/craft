#include <array>
#include <bgfx/bgfx.h>
#include <bgfx/defines.h>
#include <cstddef>
#include <cstdint>
#include <glm/ext/matrix_transform.hpp>
#include <sys/types.h>
#include <vector>
#include "chunk_mesh.h"
#include "atlas.h"
#include "direction.h"
#include "registries.h"
#include "chunk.h"
#include "chunk_vertex.h"
#include "world.h"

constexpr std::array<float, 12> BLOCK_FACE_UP = {
  0.0f, 1.0f, 0.0f,
  0.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 0.0f,
  1.0f, 1.0f, 1.0f
};

constexpr std::array<float, 12> BLOCK_FACE_DOWN = {
  0.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f,
  1.0f, 0.0f, 1.0f
};

constexpr std::array<float, 12> BLOCK_FACE_NORTH = {
  0.0f, 1.0f, 0.0f,
  1.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
};

constexpr std::array<float, 12> BLOCK_FACE_SOUTH = {
  1.0f, 1.0f, 1.0f,
  0.0f, 1.0f, 1.0f,
  1.0f, 0.0f, 1.0f,
  0.0f, 0.0f, 1.0f,
};

constexpr std::array<float, 12> BLOCK_FACE_WEST = {
  0.0f, 1.0f, 1.0f,
  0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 1.0f,
  0.0f, 0.0f, 0.0f,
};

constexpr std::array<glm::vec2, 4> BLOCK_UV {
  glm::vec2(0.0f, 1.0f),
  glm::vec2(1.0f, 1.0f),
  glm::vec2(0.0f, 0.0f),
  glm::vec2(1.0f, 0.0f)
};

constexpr std::array<std::array<int, 4>, 4> UV_PERMS {{
  {0 , 1, 2, 3},
  {2 , 3, 0, 1},
  {1, 0, 3, 2},
  {3 , 1, 2, 0},

}};

constexpr std::array<float, 12> BLOCK_FACE_EAST = {
  1.0f, 1.0f, 0.0f,
  1.0f, 1.0f, 1.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 1.0f,
};

constexpr std::array<const std::array<float, 12>*, 6> BLOCK_FACES {
  &BLOCK_FACE_UP,
  &BLOCK_FACE_DOWN,
  &BLOCK_FACE_WEST,
  &BLOCK_FACE_EAST,
  &BLOCK_FACE_NORTH,
  &BLOCK_FACE_SOUTH
};

constexpr std::array<unsigned int, 6> BLOCK_FACE_TRIANGLES = {
  0, 1, 3,
  0, 3, 2
};

ChunkMesh::ChunkMesh(const Chunk& chunk, const World& world, const Atlas& atlas, const bgfx::VertexLayout& layout) {

  std::vector<ChunkVertex> vertices;
  std::vector<uint16_t> triangles;

  for(int x = 0; x < 16; x++) {
    for(int y = 0; y < 256; y++) {
      for(int z = 0; z < 16; z++) {
        char block_id = chunk.GetBlocks()[x][y][z];
        if(block_id == 0) {
          continue;
        }
      
        glm::ivec3 block_pos = {chunk.GetPos().x * 16 + x, y, chunk.GetPos().y * 16 + z};

        for(int i = 0; i < DirectionCount; i++) {
          glm::ivec3 check_pos = block_pos + DIRECTION_VECTORS_I[i];
          if(world.GetBlock(check_pos) != 0) {
            continue;
          }
          char light_level = std::max(world.GetBlockLightLevel(check_pos), world.GetSkyLightLevel(check_pos));
          if(light_level == -1) {
            light_level = 15;
          }

          addBlockFace(
            static_cast<Direction>(i),
            block_pos,
            block_id,
            light_level,
            atlas,
            vertices,
            triangles
          );
        }
      } 
    }
  }
  vertex_buffer_ = bgfx::createVertexBuffer(bgfx::copy(vertices.data(), vertices.size() * sizeof(ChunkVertex)), layout);
  index_buffer_ = bgfx::createIndexBuffer(bgfx::copy(triangles.data(), triangles.size() * sizeof(uint16_t)));
}

ChunkMesh::~ChunkMesh() {
  bgfx::destroy(vertex_buffer_);
  bgfx::destroy(index_buffer_);
}

void addBlockFace(Direction dir, glm::ivec3 pos, char block_id, char light_level, const Atlas& atlas, std::vector<ChunkVertex>& vertices, std::vector<std::uint16_t>& triangles) {

  const std::array<float, 12>* face = BLOCK_FACES[dir];

  const Block& block = gBlockRegistry.Get(block_id);

  float uv_scale = atlas.GetTileSize();
  //glm::ivec2 chunk_pos = chunk.GetPos();
  glm::vec2 uv_offset = atlas.GetUV(block.GetTexture(dir));

  int rot = 0;
  if(block.RandomRotate(dir)) {
    rot = (pos.x * 73856093u xor pos.y * 19349663u xor pos.z * 83492791u) & 3;
  }

  int base_vertex_index = vertices.size();

  float light_level_f = (float)light_level / 15.0f;

  for(int i = 0; i < 4; i++) {
    float vert_x = (*face)[i*3] + pos.x;
    float vert_y = (*face)[i*3 + 1] + pos.y;
    float vert_z = (*face)[i*3 + 2] + pos.z;

    glm::vec2 uv = BLOCK_UV[UV_PERMS[rot][i]];
    uv *= uv_scale;
    uv += uv_offset;

    ChunkVertex vertex {
      .pos = glm::vec3(vert_x, vert_y , vert_z),
      .uv = uv,
      .light_level = light_level_f
    };
    vertices.push_back(vertex);
  }

  for(unsigned int i : BLOCK_FACE_TRIANGLES) {
    triangles.push_back(i + base_vertex_index);
  }
}


void ChunkMesh::Bind() {

  bgfx::setVertexBuffer(0, vertex_buffer_);
  bgfx::setIndexBuffer(index_buffer_);
}

