#pragma once

#include "atlas.h"
#include <ft2build.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include FT_FREETYPE_H

class Font {
 public:
  struct Character {
    int atlas_id;
    int advance;
    glm::ivec2 size;
    glm::ivec2 bearing;
  };

  Font(FT_Face& face, int height);
  const Atlas& GetAtlas() const;
  int GetHeight() const;
  const Character& GetCharacter(char c) const;
 private:
  std::unordered_map<char, Character> characters_;
  Atlas atlas_;
  Atlas createAtlas(FT_Face& face, int height);
  int height_;
};