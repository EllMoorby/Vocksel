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
        void initNoise(unsigned int size, unsigned int seed);
        void render(Shader &shader);
    private:
        std::vector<Chunk> chunks_;
        std::vector<float> noise_data_;



    };
}
#endif //WORLD_H
