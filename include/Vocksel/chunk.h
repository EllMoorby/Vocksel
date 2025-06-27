#ifndef CHUNK_H
#define CHUNK_H
#include <memory>

#include "camera.h"
#include "constants.h"
#include "shader.h"
#include "static_mesh.h"
#include "texture_atlas.h"

namespace Vocksel {
    class Chunk {
        public:
        Chunk(glm::vec3 position);
        ~Chunk();

        // Prevents copying and assigning
        Chunk(const Chunk&) = delete;
        Chunk& operator=(const Chunk&) = delete;

        // Default move and assignment operator
        Chunk(Chunk&&) noexcept = default;
        Chunk& operator=(Chunk&&) noexcept = default;

        void generateMesh();
        void render(Shader& shader);
        static void initAtlas(const std::string& atlasPath);

        static constexpr int kSize = Constants::CHUNK_SIZE;

        private:
        std::string getBlockType(int block_type);
        glm::vec3 position_;
        uint8_t voxels_[kSize][kSize][kSize];
        static TextureAtlas* texture_atlas_;
        std::unique_ptr<StaticMesh> mesh_;

    };
}


#endif //CHUNK_H
