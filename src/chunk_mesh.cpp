#include <array>
#include <bgfx/bgfx.h>
#include <bgfx/defines.h>
#include <cstddef>
#include <cstdint>
#include <glm/ext/matrix_transform.hpp>
#include "chunk_mesh.h"
#include "atlas.h"
#include "direction.h"
#include "registries.h"
#include "chunk.h"
#include "chunk_vertex.h"

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

ChunkMesh::ChunkMesh(Chunk* chunk, const Atlas* atlas, const bgfx::VertexLayout& layout)
  : chunk_(chunk),
    atlas_(atlas) {


  vertex_buffer_ = bgfx::createDynamicVertexBuffer((std::uint32_t)0, layout, BGFX_BUFFER_ALLOW_RESIZE);
  index_buffer_ = bgfx::createDynamicIndexBuffer((std::uint32_t)0, BGFX_BUFFER_ALLOW_RESIZE);
  Update();
}

ChunkMesh::~ChunkMesh() {
  bgfx::destroy(vertex_buffer_);
  bgfx::destroy(index_buffer_);
}

void addBlockFace(Direction dir, glm::ivec3 pos, char block_id, const Atlas& atlas, std::vector<ChunkVertex>& vertices, std::vector<std::uint16_t>& triangles) {

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

  for(int i = 0; i < 4; i++) {
    float vert_x = (*face)[i*3] + pos.x;
    float vert_y = (*face)[i*3 + 1] + pos.y;
    float vert_z = (*face)[i*3 + 2] + pos.z;

    glm::vec2 uv = BLOCK_UV[UV_PERMS[rot][i]];
    uv *= uv_scale;
    uv += uv_offset;

    ChunkVertex vertex {
      .pos = glm::vec3(vert_x, vert_y , vert_z),
      .uv = uv
    };
    vertices.push_back(vertex);
  }

  for(unsigned int i : BLOCK_FACE_TRIANGLES) {
    triangles.push_back(i + base_vertex_index);
  }
}

void ChunkMesh::Update() {
  if(!chunk_->HasChanged()) {
    return;
  }

  auto& blocks = chunk_->GetBlocks();

  std::vector<ChunkVertex> vertices;
  std::vector<uint16_t> triangles;


  for(int x = 0; x < 16; x++) {
    for(int y = 0; y < 256; y++) {
      for(int z = 0; z < 16; z++) {
        glm::ivec3 block_pos = glm::ivec3(x, y, z);
        char block = blocks[x][y][z];

        if(blocks[x][y][z] == 0) {
          continue;
        }

        std::array<bool, 6> visible = chunk_->GetVisibleFaces(block_pos);

        for(int i = 0; i < DirectionCount; i++) {
          if(!visible[i]) {
            continue;
          }

          addBlockFace(
            static_cast<Direction>(i),
            {block_pos.x + 16 * chunk_->GetPos().x, block_pos.y, block_pos.z + 16 * chunk_->GetPos().y},
            block,
            *atlas_,
            vertices,
            triangles
          );
        }
      } 
    }
  }

  bgfx::update(vertex_buffer_, 0, bgfx::copy(vertices.data(), vertices.size() * sizeof(ChunkVertex)));
  bgfx::update(index_buffer_, 0, bgfx::copy(triangles.data(), triangles.size() * sizeof(uint16_t)));

  chunk_->Redrawn();
}

void ChunkMesh::Bind() {

  bgfx::setVertexBuffer(0, vertex_buffer_);
  bgfx::setIndexBuffer(index_buffer_);
}

