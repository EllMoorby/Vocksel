#ifndef STATIC_MESH_H
#define STATIC_MESH_H
#include "glad/glad.h"
#include <vector>

namespace Vocksel {
    class StaticMesh {
    public:
        StaticMesh(const float* vertices, int float_count, const uint32_t* indices, int index_count, int vertexStride);
        ~StaticMesh();

        void bind();
        void unbind();
        int getIndexCount() const;
    protected:
        GLuint VAO_, VBO_, EBO_;
        int index_count_;
    };
}
#endif //STATIC_MESH_H
