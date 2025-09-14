#ifndef CHUNK_H
#define CHUNK_H
#include <memory>

#include "FastNoiseLite/FastNoiseLite.h"
#include "Vocksel/Core/constants.h"
#include "Vocksel/Graphics/Mesh/compute_mesh.h"
#include "Vocksel/Graphics/Mesh/static_mesh.h"
#include "Vocksel/Graphics/camera.h"
#include "Vocksel/Graphics/shader.h"
#include "Vocksel/Graphics/texture_atlas.h"
#include "Vocksel/Resources/resource_manager.h"
#include "density_field.h"
#include "marching_cubes.h"

namespace Vocksel {
class Chunk {
 public:
  explicit Chunk(glm::vec3 position);

  void generateTerrain(FastNoiseLite& noise);
  void generateMesh();
  void render(Shader& shader);

  glm::vec3 getPosition() { return position_; }
  DensityField& getDensityField() { return density_field_; }
  ComputeMesh& getComputeMesh() { return compute_mesh_; }

 private:
  glm::vec3 position_;
  DensityField density_field_;
  ComputeMesh compute_mesh_;
  MarchingCubes marching_cubes_;
};
}  // namespace Vocksel

#endif  // CHUNK_H
