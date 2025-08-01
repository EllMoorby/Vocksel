#ifndef CHUNK_H
#define CHUNK_H
#include <memory>

#include "Vocksel/Graphics/camera.h"
#include "Vocksel/Core/constants.h"
#include "Vocksel/Resources/resource_manager.h"
#include "Vocksel/Graphics/shader.h"
#include "Vocksel/Graphics/Mesh/static_mesh.h"
#include "Vocksel/Graphics/texture_atlas.h"

namespace Vocksel {
    class Chunk {
    public:
        Chunk(glm::vec3 position);
        ~Chunk();

        void generateMesh();
        void render(Shader& shader);
        void editVoxel(uint8_t x, uint8_t y, uint8_t z, uint8_t block_type);
        uint8_t getVoxel(uint8_t x, uint8_t y, uint8_t z);
        glm::vec3 getPosition();
        void cleanUp();
        void setNeighbor(int direction, Chunk* neighbor);
        [[nodiscard]] Chunk* getNeighbor(int direction) const;

        // Prevents copying and assigning
        Chunk(const Chunk&) = delete;
        Chunk& operator=(const Chunk&) = delete;

        // Default move and assignment operator
        Chunk(Chunk&&) noexcept = default;
        Chunk& operator=(Chunk&&) noexcept = default;
    private:
        std::string getBlockType(int block_type);
    private:
        glm::vec3 position_;
        uint8_t voxels_[Constants::CHUNK_SIZE][Constants::CHUNK_HEIGHT][Constants::CHUNK_SIZE];
        std::unique_ptr<StaticMesh> mesh_;
        Chunk* neighbors_[6] = {nullptr}; // +X, -X, +Y, -Y, +Z, -Z
    };
}


#endif //CHUNK_H
