#ifndef COMPUTE_MESH_H
#define COMPUTE_MESH_H
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <cstdint>

namespace Vocksel {

    struct Vertex {
        glm::vec4 position;
        glm::vec3 normal;
        float padding;
    };

    class ComputeMesh {
        public:
        ComputeMesh();
        ~ComputeMesh();

        uint32_t getVertexSSBO() const { return vertex_SSBO_; }
        uint32_t getIndirectBuffer() const {return indirect_buffer_; }

        void bind();
        void unbind();


        private:
        uint32_t VAO_ = 0;
        uint32_t vertex_SSBO_= 0;
        uint32_t indirect_buffer_ = 0;
    };
}

struct DrawElementsIndirectCommand {
    uint32_t count;
    uint32_t instanceCount;
    uint32_t first;
    uint32_t baseInstance;
};

#endif //COMPUTE_MESH_H
