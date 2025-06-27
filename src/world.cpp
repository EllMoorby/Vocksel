#include "Vocksel/world.h"

Vocksel::World::World() {

}

void Vocksel::World::init() {
    Chunk::initAtlas("assets/textures");
    const int worldSize = 10;
    initNoise(worldSize, 10);

    for (int x = 0; x < worldSize; ++x)
        for (int z = 0; z < worldSize; ++z)
            chunks_.emplace_back(glm::vec3(x, 0, z) * (float)Chunk::kSize);


}


void Vocksel::World::initNoise(unsigned int size, unsigned int seed = 0) {
    FastNoiseLite noise;
    noise_data_.resize(size*size);

    int index = 0;

    for (int y = 0; y < size; y++){
        for (int x = 0; x < size; x++){
            noise_data_[index++] = noise.GetNoise((float)x, (float)y);
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



