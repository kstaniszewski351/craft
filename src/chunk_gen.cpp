#include "chunk_gen.h"
#include "registries.h"

void Generate(Chunk& chunk) {
  auto dirt = gBlockRegistry.GetID("dirt");
  auto grass = gBlockRegistry.GetID("grass");

  for(int x = 0; x < 16; x++) {
    for(int y = 0; y <= 255; y++) {
      for(int z = 0; z < 16; z++) {
        char id = 0;
        if(y == 4) {
          id = grass;
          chunk.SetSkyLightLevel({x, y, z}, 15);
        }
        else if(y < 4) {
          id = dirt;
        }
        else {
          chunk.SetSkyLightLevel({x, y, z}, 15);
        }
        chunk.SetBlock(id, {x, y, z});
      }
    }
  }
}