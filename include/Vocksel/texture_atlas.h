#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H
#include <string>
#include <unordered_map>

#include "glad/glad.h"
#include "glm/glm.hpp"


namespace Vocksel {
    class TextureAtlas {
    public:
        TextureAtlas(int tile_size);
        ~TextureAtlas();

        void loadFromFolder(const std::string& path);
        GLuint getAtlasTexture() const;
        glm::vec2 getUVOffset(const std::string& textureName) const;
        float getTileScale() const;
    private:
        GLuint atlas_texture_;
        int tile_size_;
        int atlas_size_;
        std::unordered_map<std::string, glm::ivec2> texture_positions_;
    };

}

#endif //TEXTURE_ATLAS_H
