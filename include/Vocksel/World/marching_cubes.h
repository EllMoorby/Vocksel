#ifndef MARCHING_CUBES_H
#define MARCHING_CUBES_H
#include "Vocksel/Graphics/Mesh/compute_mesh.h"
#include "Vocksel/Graphics/shader.h"
#include "Vocksel/Graphics/texture_3d.h"

namespace Vocksel {
class MarchingCubes {
 public:
  MarchingCubes();
  ~MarchingCubes();

  void computeMesh(const Texture3D& density_tex, ComputeMesh& output);

  static void cleanUp();

 private:
  void createLookupTextures();
  static void createEdgeTableTexture();
  static void createTriTableTexture();

  static uint32_t tri_table_texture_;
  static uint32_t edge_table_texture_;
  static bool created_lookup_;

  Shader& compute_shader_;
};
}  // namespace Vocksel

#endif  // MARCHING_CUBES_H
