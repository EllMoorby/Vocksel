#include "Vocksel/World/world.h"

#include "tracy/Tracy.hpp"
#include "tracy/TracyOpenGL.hpp"

Vocksel::World::World(){

}

void Vocksel::World::init() {
    initDebug();

    setGenerationParams(
        0.001f,
        5,
        2.0f,
        0.5f
    );
    noise_.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise_.SetSeed(4);
    generateWorld();
}

void Vocksel::World::initDebug() {
    EngineServices::debugGUI().addPanel("World", [this]() {
        ImGui::SliderFloat("Frequency", &frequency_, 0.0f, .01f,"%.5f");
        ImGui::SliderInt("Octaves", &octaves_, 0, 10);
        ImGui::SliderFloat("Lacunarity", &lacunarity_, 0.0f, 5.f);
        ImGui::SliderFloat("Gain", &gain_, 0.0f, 1.f);
        if (ImGui::Button("Update")) {
            updateGenerationParams();
            generateWorld();
        }
    });
}



void Vocksel::World::generateWorld() {
    TracyGpuZone("Generate World")

    const int range = 2;
    const int height = 3;
    const glm::vec3 spawn_chunk = glm::floor(spawn_position_ / (float)Constants::CHUNK_SIZE);

    dirty_chunks_ = std::queue<Chunk*>();
    chunks_.clear();
    for (int z = -range; z <= range; z++) {
        for (int y = -height; y <= height; y++) {
            for (int x = -range; x <= range; x++) {
                glm::vec3 chunk_pos = spawn_chunk + glm::vec3(x, y, z);
                chunks_.emplace_back(
                    std::make_unique<Chunk>(chunk_pos * (float)Constants::CHUNK_SIZE)
                );
                dirty_chunks_.push(chunks_.back().get());
            }
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
    frequency_ = freq;
    octaves_ = octaves;
    lacunarity_ = lacunarity;
    gain_ = gain;
    updateGenerationParams();
}

void Vocksel::World::updateGenerationParams() {
    noise_.SetFrequency(frequency_);
    noise_.SetFractalOctaves(octaves_);
    noise_.SetFractalLacunarity(lacunarity_);
    noise_.SetFractalGain(gain_);
}


void Vocksel::World::update() {
    int num_processed = 0;
    while (num_processed < Constants::CHUNKS_PER_FRAME && !dirty_chunks_.empty()) {

        Chunk* chunk = dirty_chunks_.front();
        dirty_chunks_.pop();
        chunk->generateTerrain(noise_);
        chunk->getDensityField().uploadToGPU();
        chunk->generateMesh();
        num_processed++;
    }
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



