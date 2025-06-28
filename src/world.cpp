#include "Vocksel/world.h"

Vocksel::World::World(): noise_amplitude_scale_(.85f), noise_frequency_scale_(3.f) {

}

void Vocksel::World::init() {
    Chunk::initAtlas("assets/textures");

    for (int x = 0; x < Constants::WORLD_SIZE; ++x)
        for (int z = 0; z < Constants::WORLD_SIZE; ++z)
            chunks_.emplace_back(glm::vec3(x, 0, z) * (float)Chunk::kSize);

    generateWorld();
}


void Vocksel::World::generateWorld() {
    FastNoiseLite noise;
    noise.SetSeed(rand());
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    noise.SetRotationType3D(FastNoiseLite::RotationType3D_ImproveXZPlanes);

    const int chunk_size = Constants::CHUNK_SIZE;

    for (int chunk_index = 0; chunk_index < chunks_.size(); ++chunk_index) {
        auto& chunk = chunks_[chunk_index];
        glm::vec3 chunk_pos = chunk.getPosition();

        for (int x = 0; x < chunk_size; ++x) {
            for (int z = 0; z < chunk_size; ++z) {
                float world_x = chunk_pos.x + x;
                float world_z = chunk_pos.z + z;
                float noise_val = noise.GetNoise(world_x * noise_frequency_scale_, world_z * noise_frequency_scale_) * noise_amplitude_scale_;

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

    for (auto& chunk : chunks_) {
        chunk.generateMesh();
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



