#ifndef WORLD_H
#define WORLD_H
#include <queue>

#include "chunk.h"
#include "marching_cubes.h"
#include "FastNoiseLite/FastNoiseLite.h"
#include "Vocksel/Core/engine_services.h"

namespace Vocksel {
    class World {
    public:
        World();
        ~World();

        void init();
        void update();
        void render(Shader &shader);
        void generateWorld();
        void setGenerationParams(float freq, int octaves, float lacunarity, float gain);

        const glm::vec3& getSpawnPosition();
        void setSpawnPosition(glm::vec3 position);
    private:
        void initDebug();
        void updateGenerationParams();
    private:
        glm::vec3 spawn_position_ = glm::vec3(0.0f);
        std::vector<std::unique_ptr<Chunk>> chunks_;
        std::queue<Chunk*> dirty_chunks_;

        FastNoiseLite noise_;

        float frequency_ = .001f;
        int octaves_ = 5;
        float lacunarity_ = 2.0f;
        float gain_ = .5f;
    };
}
#endif //WORLD_H
