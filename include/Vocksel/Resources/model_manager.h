#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H
#include <string>
#include <unordered_map>

#include "Vocksel/Graphics/Mesh/model.h"

namespace Vocksel {
class ModelManager {
 public:
  ~ModelManager();

  void loadModel(const std::string& path, const std::string& name);
  void cleanUp();

  bool createModelFromData(const std::string& name, const float* vertices,
                           size_t vertexCount, const uint32_t* indices,
                           size_t indexCount, int vertexStride);

  Model* getModel(const std::string& name);
  std::vector<std::string> getModelNames() const;

 private:
  std::unordered_map<std::string, std::unique_ptr<Model>> models_;
};
}  // namespace Vocksel

#endif  // MODEL_MANAGER_H
