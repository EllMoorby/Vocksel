#include "Vocksel/World/chunk.h"

#include "tracy/Tracy.hpp"
#include "tracy/TracyOpenGL.hpp"
#include "Vocksel/Core/engine_services.h"



Vocksel::Chunk::Chunk(glm::vec3 position): position_(position), density_field_(glm::ivec3(Constants::CUBES_PER_CHUNK + 1), GL_R32F) {

}

void Vocksel::Chunk::generateTerrain(FastNoiseLite &noise) {

    const float voxel_size = float(Constants::CHUNK_SIZE) / float(Constants::CUBES_PER_CHUNK);

    for (int z = 0; z < int(Constants::CUBES_PER_CHUNK) + 1; z++) {
        for (int y = 0; y < int(Constants::CUBES_PER_CHUNK) + 1; y++) {
            for (int x = 0; x < int(Constants::CUBES_PER_CHUNK) + 1; x++) {
                glm::vec3 local_pos_ws = glm::vec3(x, y, z) * voxel_size;
                glm::vec3 world_pos = position_ + local_pos_ws;

                float density = noise.GetNoise(world_pos.x * 10, world_pos.y * 10 , world_pos.z * 10);


                density_field_.setVoxel(x, y, z, density);
            }
        }
    }

    density_field_.uploadToGPU();
}



void Vocksel::Chunk::generateMesh() {
    marching_cubes_.computeMesh(density_field_.getTexture(), compute_mesh_);
}

void Vocksel::Chunk::render(Shader &shader) {
    TracyGpuZone("Render Chunk");
    ZoneScoped;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position_));

    shader.setMat4("model", model);

    compute_mesh_.bind();

    glDrawElements(GL_TRIANGLES, compute_mesh_.getIndexCount(), GL_UNSIGNED_INT, nullptr);

    compute_mesh_.unbind();
}




