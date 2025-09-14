#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include "glad/glad.h"

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

namespace Vocksel {
class TextureAtlas {
 public:
  TextureAtlas(int tile_size);
  ~TextureAtlas();

  void loadFromFolder(const std::string& path);
  GLuint getAtlasTexture() const;
  glm::vec2 getUVOffset(const std::string& textureName) const;
  float getTileScale() const;

  void cleanUp();

 private:
  GLuint atlas_texture_;
  int tile_size_;
  int atlas_size_;
  std::unordered_map<std::string, glm::ivec2> texture_positions_;
};

}  // namespace Vocksel

#endif  // TEXTURE_ATLAS_H
