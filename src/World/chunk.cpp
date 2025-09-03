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

    for (int z = 0; z <= Constants::CUBES_PER_CHUNK; z++) {
        for (int x = 0; x <= Constants::CUBES_PER_CHUNK; x++) {
            glm::vec3 world = (position_ + glm::vec3(x, 0, z) * voxel_size);

            float base_height = noise.GetNoise(world.x * 10.f, world.z * 10.f) * 32.0f + 16.0f;

            for (int y = 0; y <= Constants::CUBES_PER_CHUNK; y++) {
                glm::vec3 local_pos_ws = glm::vec3(x, y, z) * voxel_size;
                glm::vec3 world_pos = position_ + local_pos_ws;

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
    ZoneScoped;


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position_));

    shader.setMat4("model", model);


    compute_mesh_.bind();

    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, compute_mesh_.getIndirectBuffer());

    glDrawArraysIndirect(GL_TRIANGLES, nullptr);

    compute_mesh_.unbind();
}




