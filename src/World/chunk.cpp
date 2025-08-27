#include "Vocksel/World/chunk.h"

#include "tracy/Tracy.hpp"
#include "tracy/TracyOpenGL.hpp"
#include "Vocksel/Core/engine_services.h"



Vocksel::Chunk::Chunk(glm::vec3 position): position_(position), density_field_(glm::ivec3(Constants::CUBES_PER_CHUNK + 1), GL_R32F) {

}

void Vocksel::Chunk::generateTerrain(FastNoiseLite &noise) {
    ZoneScoped;
    TracyGpuZone("GenerateTerrain");

    const float voxel_size = float(Constants::CHUNK_SIZE) / float(Constants::CUBES_PER_CHUNK);

    for (int z = 0; z < int(Constants::CUBES_PER_CHUNK) + 1; z++) {
        for (int y = 0; y < int(Constants::CUBES_PER_CHUNK) + 1; y++) {
            for (int x = 0; x < int(Constants::CUBES_PER_CHUNK) + 1; x++) {
                glm::vec3 local_pos_ws = glm::vec3(x, y, z) * voxel_size;
                glm::vec3 world_pos = position_ + local_pos_ws;

                // Base terrain height using 2D noise
                float base_height = noise.GetNoise(world_pos.x * 10.f, world_pos.z * 10.f) * 32.0f + 16.0f;

                // Density based on height comparison
                float density = world_pos.y - base_height;

                density_field_.setVoxel(x, y, z, density);
            }
        }
    }

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

    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, compute_mesh_.getIndirectBuffer());
    glDrawArraysIndirect(GL_TRIANGLES, nullptr);

    compute_mesh_.unbind();
}




