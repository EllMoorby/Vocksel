#ifndef WORLD_H
#define WORLD_H
#include "chunk.h"
#include "marching_cubes.h"
#include "FastNoiseLite/FastNoiseLite.h"

namespace Vocksel {
    class World {
    public:
        World();
        ~World();

        void init();
        void render(Shader &shader);
        void generateWorld();
        void setGenerationParams(float freq, int octaves, float lacunarity, float gain);

        const glm::vec3& getSpawnPosition();
        void setSpawnPosition(glm::vec3 position);
    private:
        glm::vec3 spawn_position_ = glm::vec3(0.0f);
        std::vector<std::unique_ptr<Chunk>> chunks_;

        FastNoiseLite noise_;
    };
}
#endif //WORLD_H
