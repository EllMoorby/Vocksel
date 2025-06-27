#ifndef CHUNK_H
#define CHUNK_H
#include <memory>

#include "camera.h"
#include "constants.h"
#include "shader.h"
#include "static_mesh.h"

namespace Vocksel {
    class Chunk {
        public:
        Chunk(glm::vec3 position);
        ~Chunk();

        // Delete copy constructor and copy assignment operator explicitly:
        Chunk(const Chunk&) = delete;
        Chunk& operator=(const Chunk&) = delete;

        // Default move constructor and move assignment operator:
        Chunk(Chunk&&) noexcept = default;
        Chunk& operator=(Chunk&&) noexcept = default;

        void generateMesh();
        void render(Shader& shader, const Camera& camera);

        static constexpr int kSize = Constants::CHUNK_SIZE;

        private:
        glm::vec3 position_;
        uint8_t voxels_[kSize][kSize][kSize];
        std::unique_ptr<StaticMesh> mesh_;

    };
}


#endif //CHUNK_H
