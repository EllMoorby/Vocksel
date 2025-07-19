#ifndef WORLD_H
#define WORLD_H
#include "Vocksel/chunk.h"
#include "FastNoiseLite/FastNoiseLite.h"

namespace Vocksel {
    class World {
    public:
        World();
        ~World();

        void init();
        void render(Shader &shader);
        void generateWorld();

        const glm::vec3& getSpawnPosition();
        void setSpawnPosition(glm::vec3 position);
        uint8_t getBlockAtWorldPos(int x, int y, int z);
    public:
        int noise_num_octaves_;
        float noise_freq_per_octave_, noise_ampl_per_octave_, noise_frequency_;
    private:
        void generateSpawnPosition();
    private:
        glm::vec3 spawn_position_ = glm::vec3(0.0f);
        std::vector<Chunk> chunks_;
        std::vector<float> noise_data_;
    };
}
#endif //WORLD_H
