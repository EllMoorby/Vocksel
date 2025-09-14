#ifndef MODEL_H
#define MODEL_H
#include <memory>
#include <string>

#include "Vocksel/Graphics/Mesh/static_mesh.h"
#include "Vocksel/Graphics/shader.h"

namespace Vocksel {
class Model {
 public:
  Model(const std::string& path = "");
  ~Model();

  void render(Shader& shader);
  void setTransformMatrix(const glm::mat4& transform);
  void addMesh(std::unique_ptr<StaticMesh> mesh);
  void cleanUp();

  bool loadFromFile(std::string path);
  bool createModelFromData(const std::string& name, const float* vertices,
                           size_t vertexCount, const uint32_t* indices,
                           size_t indexCount, int vertexStride);

 private:
  std::vector<std::unique_ptr<StaticMesh>> meshes_;
  glm::mat4 transform_ = glm::mat4(1.0f);
};
}  // namespace Vocksel

#endif  // MODEL_H
