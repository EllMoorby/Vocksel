#ifndef CHUNK_H
#define CHUNK_H
#include <memory>

#include "density_field.h"
#include "marching_cubes.h"
#include "FastNoiseLite/FastNoiseLite.h"
#include "Vocksel/Graphics/camera.h"
#include "Vocksel/Core/constants.h"
#include "Vocksel/Resources/resource_manager.h"
#include "Vocksel/Graphics/shader.h"
#include "Vocksel/Graphics/Mesh/static_mesh.h"
#include "Vocksel/Graphics/texture_atlas.h"
#include "Vocksel/Graphics/Mesh/compute_mesh.h"

namespace Vocksel {
    class Chunk {
    public:
        Chunk(glm::vec3 position);

        void generateTerrain(FastNoiseLite& noise);
        void generateMesh();
        void render(Shader& shader);

        glm::vec3 getPosition() {return position_;}
        DensityField& getDensityField() {return density_field_;}

    private:
        glm::vec3 position_;
        DensityField density_field_;
        ComputeMesh compute_mesh_;
        MarchingCubes marching_cubes_;

    };
}


#endif //CHUNK_H
