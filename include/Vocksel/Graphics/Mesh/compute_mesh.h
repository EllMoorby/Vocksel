#ifndef COMPUTE_MESH_H
#define COMPUTE_MESH_H
#include "glad/glad.h"
#include <cstdint>

namespace Vocksel {
    class ComputeMesh {
        public:
        ComputeMesh();
        ~ComputeMesh();

        void updateFromGPU();

        uint32_t getVertexSSBO() const { return vertex_SSBO_; }
        uint32_t getIndexSSBO() const { return index_SSBO_; }
        uint32_t getCounterBuffer() const { return counter_buffer_; }
        uint32_t readTriangleCount() const;

        void bind();
        void unbind();
        int getIndexCount() const { return triangle_count_ * 3; };

        private:
        uint32_t VAO_;
        uint32_t VBO_;
        uint32_t EBO_;
        uint32_t vertex_SSBO_;
        uint32_t index_SSBO_;
        uint32_t counter_buffer_;
        int triangle_count_ = -1;
    };
}

#endif //COMPUTE_MESH_H
