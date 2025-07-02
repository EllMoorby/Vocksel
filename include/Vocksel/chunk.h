#ifndef CHUNK_H
#define CHUNK_H
#include <memory>

#include "camera.h"
#include "constants.h"
#include "resource_manager.h"
#include "shader.h"
#include "static_mesh.h"
#include "texture_atlas.h"

namespace Vocksel {
    class Chunk {
    public:
        Chunk(glm::vec3 position, ResourceManager& resource_manager);
        ~Chunk();

        void generateMesh();
        void render(Shader& shader);
        void editVoxel(uint8_t x, uint8_t y, uint8_t z, uint8_t block_type);
        glm::vec3 getPosition();
        void cleanUp();

        // Prevents copying and assigning
        Chunk(const Chunk&) = delete;
        Chunk& operator=(const Chunk&) = delete;

        // Default move and assignment operator
        Chunk(Chunk&&) noexcept = default;
        Chunk& operator=(Chunk&&) noexcept = default;
    public:
        static constexpr int kSize = Constants::CHUNK_SIZE;
    private:
        std::string getBlockType(int block_type);
    private:
        glm::vec3 position_;
        ResourceManager& resource_manager_;
        uint8_t voxels_[kSize][kSize][kSize];
        std::unique_ptr<StaticMesh> mesh_;
    };
}


#endif //CHUNK_H
