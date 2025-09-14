#ifndef RESOURCE__MANAGER_H
#define RESOURCE__MANAGER_H
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Vocksel/Graphics/shader.h"
#include "Vocksel/Graphics/texture.h"
#include "Vocksel/Graphics/texture_atlas.h"

namespace Vocksel {

class ResourceManager {
 public:
  ResourceManager();
  ~ResourceManager();

  // Prevent copying
  ResourceManager(const ResourceManager&) = delete;
  ResourceManager& operator=(const ResourceManager&) = delete;

  void init();

  void loadShader(std::string name, const char* vertex_path,
                  const char* fragment_path);
  void loadShader(std::string name, const char* comp_path);
  void loadTexture(std::string name, const char* path);
  void loadBlockAtlas(const std::string& path);
  void reloadShader(std::string name);
  void cleanUp();

  Shader& getShader(const std::string& name);
  Texture& getTexture(const std::string& name);
  std::vector<std::string> getTextureNames();
  TextureAtlas& getBlockAtlas();

 private:
  void initDebug();

 private:
  std::unordered_map<std::string, std::unique_ptr<Shader>> shaders_;
  std::unordered_map<std::string, std::unique_ptr<Texture>> textures_;
  std::unique_ptr<TextureAtlas> blocks_atlas_;

  bool initialized_ = false;
};
}  // namespace Vocksel

#endif  // RESOURCE__MANAGER_H
