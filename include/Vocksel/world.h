#ifndef WORLD_H
#define WORLD_H
#include "Vocksel/chunk.h"
#include "FastNoiseLite/FastNoiseLite.h"

namespace Vocksel {
    class World {
    public:
        World(ResourceManager& resource_manager);
        ~World();

        void init();
        void render(Shader &shader);
        void generateWorld();
    public:
        float noise_amplitude_scale_, noise_frequency_scale_;
    private:
        ResourceManager& resource_manager_;
        std::vector<Chunk> chunks_;
        std::vector<float> noise_data_;
    };
}
#endif //WORLD_H
