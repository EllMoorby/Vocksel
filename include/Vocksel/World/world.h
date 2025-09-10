#ifndef WORLD_H
#define WORLD_H
#include <queue>

#include "chunk.h"
#include "marching_cubes.h"
#include "FastNoiseLite/FastNoiseLite.h"
#include "Vocksel/Core/engine_services.h"


struct IVec3Hash {
    std::size_t operator()(const glm::ivec3& v) const noexcept {
        std::size_t h1 = std::hash<int>()(v.x);
        std::size_t h2 = std::hash<int>()(v.y);
        std::size_t h3 = std::hash<int>()(v.z);

        std::size_t seed = h1;
        seed ^= h2 + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
        seed ^= h3 + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
        return seed;
    }
};

struct IVec3Eq {
    bool operator()(const glm::ivec3& a, const glm::ivec3& b) const noexcept {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }
};



namespace Vocksel {
    class World {
    public:
        World();
        ~World();

        void init();
        void update(glm::vec3 position);
        void render(Shader &shader);
        void clearWorld();
        void updateChunksAroundPosition(glm::vec3 position);
        void setGenerationParams(float freq, int octaves, float lacunarity, float gain);

        const glm::vec3& getSpawnPosition();
        void setSpawnPosition(glm::vec3 position);
    public:
        static float isolevel;
    private:
        void initDebug();
        void updateGenerationParams();
    private:
        glm::vec3 spawn_position_ = glm::vec3(0.0f);
        std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, IVec3Hash, IVec3Eq> chunk_map_;
        std::queue<Chunk*> dirty_chunks_;

        FastNoiseLite noise_;

        float frequency_ = .0007f;
        int octaves_ = 5;
        float lacunarity_ = 2.0f;
        float gain_ = .5f;

        glm::vec3 light_pos_ = glm::vec3(100.0, 200.0, 100.0);
        glm::vec3 light_color_ = glm::vec3(1.0, 1.0, 1.0);
        glm::vec3 ambient_color_ = glm::vec3(0.2, 0.2, 0.2);

        glm::vec3 grass_color_ = glm::vec3(0.2, 0.6, 0.1);
        glm::vec3 dirt_color_ = glm::vec3(0.5, 0.4, 0.2);
        glm::vec3 rock_color_ = glm::vec3(0.4, 0.4, 0.4);

        float snow_height_ = -9.f;
        float snow_transition_ = 4.f;
    };
}
#endif //WORLD_H
