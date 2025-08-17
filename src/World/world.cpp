#include "Vocksel/World/world.h"

#include "tracy/Tracy.hpp"
#include "tracy/TracyOpenGL.hpp"

Vocksel::World::World(){

}

void Vocksel::World::init() {
    setGenerationParams(
        0.001f,
        5,
        2.0f,
        0.5f
    );
    noise_.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise_.SetSeed(5);
    generateWorld();
}


void Vocksel::World::generateWorld() {
    const int range = 1;
    const glm::vec3 spawn_chunk = glm::floor(spawn_position_ / (float)Constants::CUBES_PER_CHUNK);

    chunks_.clear();
    for (int z = -range; z <= range; z++) {
            for (int x = -range; x <= range; x++) {
                glm::vec3 chunk_pos = spawn_chunk + glm::vec3(x, 0, z);
                chunks_.emplace_back(
                    std::make_unique<Chunk>(chunk_pos * (float)Constants::CUBES_PER_CHUNK)
                );
                chunks_.back()->generateTerrain(noise_);
                chunks_.back()->generateMesh();
            }
    }
}


const glm::vec3 &Vocksel::World::getSpawnPosition() {
    return spawn_position_;
}

void Vocksel::World::setSpawnPosition(glm::vec3 position) {
    spawn_position_ = position;
}

void Vocksel::World::setGenerationParams(float freq, int octaves, float lacunarity, float gain) {
    noise_.SetFrequency(freq);
    noise_.SetFractalOctaves(octaves);
    noise_.SetFractalLacunarity(lacunarity);
    noise_.SetFractalGain(gain);
}



void Vocksel::World::render(Shader &shader) {
    ZoneScoped;
    TracyGpuZone("Render World");

    shader.use();
    // Render chunks
    for (auto& chunk : chunks_) {
        chunk->render(shader);
    }

}

Vocksel::World::~World() {
}



