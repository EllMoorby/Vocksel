#include "Vocksel/World/chunk.h"

#include "tracy/Tracy.hpp"
#include "Vocksel/Core/engine_services.h"



Vocksel::Chunk::Chunk(glm::vec3 position): position_(position), density_field_(glm::ivec3(Constants::CHUNK_SIZE), GL_R32F) {

}

void Vocksel::Chunk::generateTerrain(FastNoiseLite &noise) {

    for (int z = 0; z < Constants::CHUNK_SIZE; z++) {
        for (int y = 0; y < Constants::CHUNK_SIZE; y++) {
            for (int x = 0; x < Constants::CHUNK_SIZE; x++) {
                glm::vec3 world_pos = position_ + glm::vec3(x, y, z);
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
    glDisable(GL_CULL_FACE);
    ZoneScoped;
    shader.use();


    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position_));

    shader.setMat4("model", model);


    compute_mesh_.bind();
    {
        ZoneScoped;
        glDrawElements(GL_TRIANGLES, compute_mesh_.getIndexCount(), GL_UNSIGNED_INT, nullptr);

    }
    compute_mesh_.unbind();
    glEnable(GL_CULL_FACE);
}




