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
    private:
        float noise_scale_x, noise_scale_y;
        const int world_size_;
        std::vector<Chunk> chunks_;
        std::vector<float> noise_data_;
    };
}
#endif //WORLD_H
