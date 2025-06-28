#include "Vocksel/world.h"

Vocksel::World::World(): world_size_(10), noise_scale_x(1.f), noise_scale_y(1.f) {

}

void Vocksel::World::init() {
    Chunk::initAtlas("assets/textures");

    for (int x = 0; x < world_size_; ++x)
        for (int z = 0; z < world_size_; ++z)
            chunks_.emplace_back(glm::vec3(x, 0, z) * (float)Chunk::kSize);

    generateWorld();
    for (auto& chunk : chunks_) {
        chunk.generateMesh();
    }

}


void Vocksel::World::generateWorld() {
    FastNoiseLite noise;
    noise.SetSeed(2343545);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);

    const int chunk_size = Constants::CHUNK_SIZE;

    for (int chunk_index = 0; chunk_index < chunks_.size(); ++chunk_index) {
        auto& chunk = chunks_[chunk_index];
        glm::vec3 chunk_pos = chunk.getPosition();

        for (int x = 0; x < chunk_size; ++x) {
            for (int z = 0; z < chunk_size; ++z) {
                float world_x = chunk_pos.x + x;
                float world_z = chunk_pos.z + z;
                float noise_val = noise.GetNoise(world_x * noise_scale_x, world_z * noise_scale_y);

                // Convert to height
                float height_f = (noise_val + 1.0f) * 0.5f * chunk_size;
                int height = static_cast<int>(height_f);

                for (int y = 0; y < chunk_size; ++y) {
                    uint8_t block_type = (y < height) ? 3 : 0;
                    chunk.editVoxel(x, y, z, block_type);
                }
            }
        }
    }
}


void Vocksel::World::render(Shader &shader) {
    // Render chunks
    for (auto& chunk : chunks_) {
        chunk.render(shader);
    }
}

Vocksel::World::~World() {

}



