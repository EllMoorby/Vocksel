#ifndef DENSITY_FIELD_H
#define DENSITY_FIELD_H
#include <vector>

#include "Vocksel/Graphics/texture_3d.h"
#include "glm/glm.hpp"

namespace Vocksel {
class DensityField {
 public:
  DensityField(glm::ivec3 size, uint32_t gpu_format);

  void init(glm::ivec3 size);
  void setVoxel(int x, int y, int z, float value);
  void uploadToGPU();

  [[nodiscard]] bool isInitialized() const { return texture_.getId() != 0; }

  [[nodiscard]] const Texture3D& getTexture() const { return texture_; }

  [[nodiscard]] float getVoxel(int x, int y, int z) const;

 private:
  [[nodiscard]] inline size_t index(int x, int y, int z) const;

  Texture3D texture_;
  glm::ivec3 size_;
  std::vector<float> voxels_;
};
}  // namespace Vocksel
#endif  // DENSITY_FIELD_H
