#include <algorithm>
#include <array>
#include <glm/ext/matrix_transform.hpp>
#include "chunk_mesh.h"
#include "atlas.h"
#include "block_registry.h"
#include "buffer.h"
#include "chunk.h"
#include "texture.h"




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

constexpr std::array<unsigned int, 6> BLOCK_FACE_TRIANGLES = {
  0, 1, 3,
  0, 3, 2
};

ChunkMesh::ChunkMesh(Chunk& chunk, const Atlas& atlas) : chunk_(chunk), atlas_(atlas) {
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  vbo_.Bind(GL_ARRAY_BUFFER);
  ebo_.Bind(GL_ELEMENT_ARRAY_BUFFER);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ChunkVertex), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ChunkVertex), (void*)(sizeof(float) * 3));
  Update();
}

ChunkMesh::~ChunkMesh() {

}

void ChunkMesh::Update() {
  if(!chunk_.HasChanged()) {
    return;
  }

  auto& blocks = chunk_.GetBlocks();

  vertices.clear();
  triangles.clear();
  for(int x = 0; x < 16; x++) {
    for(int y = 0; y < 256; y++) {
      for(int z = 0; z < 16; z++) {
        glm::ivec3 block_pos = glm::ivec3(x, y, z);
        char block = blocks[x][y][z];

        if(blocks[x][y][z] != 0) {
          if(y == 15 || blocks[x][y+1][z] == 0) {
            addFace(Up, block_pos, block);
          }
          if(y == 0 || blocks[x][y-1][z] == 0) {
            addFace(Down, block_pos, block);
          }
          if(
            (x == 0 && chunk_.GetNeighbor(West) != nullptr && chunk_.GetNeighbor(West)->GetBlocks()[15][y][z] == 0) ||
            (x > 0 && blocks[x-1][y][z] == 0)
          ) {
            addFace(Left, block_pos, block);
          }
          if(
            (x == 15 && chunk_.GetNeighbor(East) != nullptr && chunk_.GetNeighbor(East)->GetBlocks()[0][y][z] == 0) ||
            (x < 15 && blocks[x+1][y][z] == 0)
          ) {
            addFace(Right, block_pos, block);
          }
          if(
            (z == 0 && chunk_.GetNeighbor(North) != nullptr && chunk_.GetNeighbor(North)->GetBlocks()[x][y][15] == 0) ||
            (z > 0 && blocks[x][y][z-1] == 0)
          ) {
            addFace(Front, block_pos, block);
          }
          if(
            (z == 15 && chunk_.GetNeighbor(South) != nullptr && chunk_.GetNeighbor(South)->GetBlocks()[x][y][0] == 0) ||
            (z < 15 && blocks[x][y][z+1] == 0)
          ) {
            addFace(Back, block_pos, block);
          }
          
        }
      } 
    }
  }

  vbo_.Data(sizeof(ChunkVertex) * vertices.size(), vertices.data());
  ebo_.Data(sizeof(unsigned int) * triangles.size(), triangles.data());

  chunk_.Redrawn();
}

void ChunkMesh::Draw() {
  if(vertices.empty()) {
    return;
  }

  glBindVertexArray(vao_);

  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(triangles.size()), GL_UNSIGNED_INT, 0);
  //glDrawArrays(GL_TRIANGLE_STRIP, 0, triangles.size());
}

void ChunkMesh::addFace(Direction dir, glm::ivec3 pos, char block_id) {

  const std::array<float, 12>* face;

  switch (dir) {
    case Up:
      face = &BLOCK_FACE_UP;
      break;
    case Down:
      face = &BLOCK_FACE_DOWN;
      break;
    case Front:
      face = &BLOCK_FACE_NORTH;
      break;
    case Back:
      face = &BLOCK_FACE_SOUTH;
      break;
    case Left:
      face = &BLOCK_FACE_WEST;
      break;
    case Right:
      face = &BLOCK_FACE_EAST;
      break;
    default:
      return;
  }

  const Block& block = gBlockRegistry.GetBlock(block_id);

  float uv_scale = atlas_.GetTileSize();
  glm::ivec2 chunk_pos = chunk_.GetPos();
  glm::vec2 uv_offset = atlas_.GetUV(block.GetTexture(dir));

  int rot = 0;
  if(block.RandomRotate(dir)) {
    rot = (pos.x * 73856093u xor pos.y * 19349663u xor pos.z * 83492791u) % 4;
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
      .pos = glm::vec3(vert_x + chunk_pos.x * 16, vert_y , vert_z  + chunk_pos.y * 16),
      .uv = uv
    };
    vertices.push_back(vertex);
  }

  for(unsigned int i : BLOCK_FACE_TRIANGLES) {
    triangles.push_back(i + base_vertex_index);
  }
}