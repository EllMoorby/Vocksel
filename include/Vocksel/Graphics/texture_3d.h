#ifndef TEXTURE_3D_H
#define TEXTURE_3D_H
#include "glad/glad.h"
#include "glm/glm.hpp"

namespace Vocksel {
    class Texture3D {
        public:
        Texture3D() :  size_(0), format_(0) {}
        Texture3D(const float *data, glm::ivec3 size, uint32_t format = GL_R32F);
        ~Texture3D();
        void createIfNeeded(const float *data, glm::ivec3 size, uint32_t format = GL_R32F);
        void createFromData(const float *data, glm::ivec3 size, uint32_t format = GL_R32F);
        void bind();
        void unbind();

        uint32_t getId() const {return texture_id_;};
        glm::ivec3 getSize() const {return size_;};

        void cleanUp();

        private:
        glm::ivec3 size_ = glm::ivec3(0);
        uint32_t texture_id_ = 0;
        uint32_t format_ = GL_R32F;
    };
}
#endif //TEXTURE_3D_H
