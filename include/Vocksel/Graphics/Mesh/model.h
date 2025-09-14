#ifndef MODEL_H
#define MODEL_H
#include <memory>
#include <string>

#include "Vocksel/Graphics/Mesh/static_mesh.h"
#include "Vocksel/Graphics/shader.h"

namespace Vocksel {
class Model {
 public:
  Model();
  ~Model();

  void render(Shader& shader);
  void setTransformMatrix(const glm::mat4& transform);
  void addMesh(std::unique_ptr<StaticMesh> mesh);
  void cleanUp();

  bool loadFromFile(std::string path);

 private:
  std::vector<std::unique_ptr<StaticMesh>> meshes_;
  glm::mat4 transform_ = glm::mat4(1.0f);
};
}  // namespace Vocksel

#endif  // MODEL_H
