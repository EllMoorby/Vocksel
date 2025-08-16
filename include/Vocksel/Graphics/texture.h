#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>

namespace Vocksel {
    class Texture {
        public:
        Texture();
        Texture(const char* path);
        ~Texture();

        void createEmptyTexture(int width, int height);
        void loadTexture(const char* path);
        void bind();
        void unbind();

        void cleanUp();
        private:
        int width_ = 0, height_ = 0;
        uint32_t texture_id_ = 0;

    };
}

#endif //TEXTURE_H
