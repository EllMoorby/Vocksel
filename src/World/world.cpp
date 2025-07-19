#include "Vocksel/World/world.h"

Vocksel::World::World(): noise_num_octaves_(5), noise_freq_per_octave_(0.95f),
                         noise_ampl_per_octave_(0.43f), noise_frequency_(0.021f) {

}

void Vocksel::World::init() {
    for (int x = 0; x < Constants::WORLD_SIZE; x++)
        for (int z = 0; z < Constants::WORLD_SIZE; z++)
            chunks_.emplace_back(glm::vec3(x, 0, z) * (float)Constants::CHUNK_SIZE);

    for (int x = 0; x < Constants::WORLD_SIZE; x++) {
        for (int z = 0; z < Constants::WORLD_SIZE; z++) {
            Chunk& chunk = chunks_[x * Constants::WORLD_SIZE + z];
            if (x < Constants::WORLD_SIZE - 1)
                chunk.setNeighbor(0, &chunks_[(x + 1) * Constants::WORLD_SIZE + z]); // +X
            if (x > 0)
                chunk.setNeighbor(1, &chunks_[(x - 1) * Constants::WORLD_SIZE + z]); // -X
            if (z < Constants::WORLD_SIZE - 1)
                chunk.setNeighbor(4, &chunks_[x * Constants::WORLD_SIZE + (z + 1)]); // +Z
            if (z > 0)
                chunk.setNeighbor(5, &chunks_[x * Constants::WORLD_SIZE + (z - 1)]); // -Z
        }
    }
    generateWorld();
}

uint8_t Vocksel::World::getBlockAtWorldPos(int x, int y, int z) {
    if (y >= (int)Constants::CHUNK_HEIGHT) return 0; // Treat up as air
    if (y < 0) return 1; // Treat below 0 as solid

    int chunkX = x / Constants::CHUNK_SIZE;
    int chunkZ = z / Constants::CHUNK_SIZE;
    int localX = x % Constants::CHUNK_SIZE;
    int localZ = z % Constants::CHUNK_SIZE;

    if (chunkX < 0 || chunkZ < 0 || chunkX >= Constants::WORLD_SIZE || chunkZ >= Constants::WORLD_SIZE)
        return 0; // Treat out-of-bounds as air

    Chunk& chunk = chunks_[chunkX * Constants::WORLD_SIZE + chunkZ];
    return chunk.getVoxel(localX, y, localZ);
}


void Vocksel::World::generateWorld() {
    FastNoiseLite noise;
    noise.SetSeed(rand());
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    noise.SetRotationType3D(FastNoiseLite::RotationType3D_ImproveXZPlanes);

    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFractalOctaves(noise_num_octaves_);
    noise.SetFractalLacunarity(noise_freq_per_octave_);
    noise.SetFractalGain(noise_ampl_per_octave_);
    noise.SetFrequency(noise_frequency_);

    const int chunk_size = Constants::CHUNK_SIZE;

    for (int chunk_index = 0; chunk_index < chunks_.size(); ++chunk_index) {
        auto& chunk = chunks_[chunk_index];
        glm::vec3 chunk_pos = chunk.getPosition();

        for (int x = 0; x < chunk_size; x++) {
            for (int z = 0; z < chunk_size; z++) {
                float world_x = chunk_pos.x + x;
                float world_z = chunk_pos.z + z;
                float noise_val = noise.GetNoise(world_x, world_z);

                // Convert to height
                //float modified_noise = std::pow((noise_val + 1.0f) * 0.5f, 0.7f); // 0.7 = smoother hills
                float height_f = (noise_val + 1.0f) * 0.5f * Constants::CHUNK_HEIGHT;
                int height = static_cast<int>(height_f);

                for (int y = 0; y < Constants::CHUNK_HEIGHT; ++y) {
                    uint8_t block_type = (y < height) ? 1 : 0;
                    chunk.editVoxel(x, y, z, block_type);
                }
            }
        }
    }

    for (auto& chunk : chunks_) {
        chunk.generateMesh();
    }

    generateSpawnPosition();
}

void Vocksel::World::generateSpawnPosition() {
    if (chunks_.empty()) return;

    const int chunk_size = Constants::CHUNK_SIZE;

    std::vector<glm::vec3> possible_spawns;

    auto& chunk = chunks_[0];
    for (int x = 0; x < chunk_size; x++) {
        for (int z = 0; z < chunk_size; z++) {
            bool found_possible = false;
            uint8_t last_type = 200;
            for (int y = Constants::CHUNK_HEIGHT - 1; y >= 0 ; y--) {
                if (chunk.getVoxel(x,y,z) == 1 && last_type == 0 && found_possible == false) {
                    possible_spawns.emplace_back(glm::vec3(x,y + 1,z)); // This only works if we are checking the corner most chunk
                    found_possible = true;
                }
                last_type = chunk.getVoxel(x,y,z);
            }
        }
    }

    int rand_pos = rand() % possible_spawns.size();
    spawn_position_ = possible_spawns[rand_pos];

}


const glm::vec3 &Vocksel::World::getSpawnPosition() {
    return spawn_position_;
}

void Vocksel::World::setSpawnPosition(glm::vec3 position) {
    spawn_position_ = position;
}




void Vocksel::World::render(Shader &shader) {
    // Render chunks
    for (auto& chunk : chunks_) {
        chunk.render(shader);
    }
}

Vocksel::World::~World() {
}



