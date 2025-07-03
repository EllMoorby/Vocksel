#include "Vocksel/texture_atlas.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <vector>
#include "stb/stb_image.h"

Vocksel::TextureAtlas::TextureAtlas(int tile_size): atlas_texture_(0), tile_size_(tile_size), atlas_size_(0) {}

void Vocksel::TextureAtlas::loadFromFolder(const std::string &path) {
    if (!glfwGetCurrentContext()) {
        throw std::runtime_error("No OpenGL context for texture loading");
    }

    std::cout << "Created texture atlas (ID: " << atlas_texture_ << ")\n";

    std::vector<std::string> texture_files;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg") {
            texture_files.push_back(entry.path().string());
        }
    }

    // Calculate minimum atlas size that fits all textures
    int num_textures = texture_files.size();
    int textures_per_row = 1;

    // Find smallest power-of-two that fits all texture
    while (textures_per_row * textures_per_row < num_textures) {
        textures_per_row++;
    }

    atlas_size_ = textures_per_row * tile_size_;

    std::cout << "Creating atlas size: " << atlas_size_ << "x" << atlas_size_ << std::endl;

    // Create the atlas buffer
    std::vector<unsigned char> atlas_data(atlas_size_ * atlas_size_ * 4, 0);


    int x_pos = 0, y_pos = 0;
    for (const auto& file : texture_files) {
        int w, h, c;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* data = stbi_load(file.c_str(), &w, &h, &c, 4);
        if (!data) {
            std::cerr << "Failed to load texture atlas file: " << file << std::endl;
            std::cerr << stbi_failure_reason() << std::endl;
            continue;
        };

        // Copy to atlas
        for (int y = 0; y < tile_size_; y++) {
            for (int x = 0; x < tile_size_; x++) {
                int atlasIdx = ((y_pos + y) * atlas_size_ + (x_pos + x)) * 4;
                int texIdx = (y * tile_size_ + x) * 4;
                for (int i = 0; i < 4; i++) {
                    atlas_data[atlasIdx + i] = data[texIdx + i];
                }
            }
        }

        std::string name = std::filesystem::path(file).stem().string();
        texture_positions_[name] = {x_pos, y_pos};

        x_pos += tile_size_;
        if (x_pos >= atlas_size_) {
            x_pos = 0;
            y_pos += tile_size_;
        }

        stbi_image_free(data);
    }
//
    glGenTextures(1, &atlas_texture_);
    glBindTexture(GL_TEXTURE_2D, atlas_texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlas_size_, atlas_size_, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlas_data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

GLuint Vocksel::TextureAtlas::getAtlasTexture() const {
    return atlas_texture_;
}

float Vocksel::TextureAtlas::getTileScale() const {
    return 1.0f / (static_cast<float>(atlas_size_)/static_cast<float>(tile_size_));
}


glm::vec2 Vocksel::TextureAtlas::getUVOffset(const std::string &textureName) const {
    auto it = texture_positions_.find(textureName);
    if (it == texture_positions_.end()) return {0,0};

    glm::vec2 offset(it->second.x, it->second.y);
    offset /= static_cast<float>(atlas_size_);

    return offset;
}

void Vocksel::TextureAtlas::cleanUp() {
    if (atlas_texture_ != 0) {
        glDeleteTextures(1, &atlas_texture_);
        atlas_texture_ = 0;
    }

    texture_positions_.clear();

    atlas_size_ = 0;
}

Vocksel::TextureAtlas::~TextureAtlas() {
    cleanUp();
}




