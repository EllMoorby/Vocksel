#include "Vocksel/Graphics/texture.h"

#include <iostream>

#include "stb/stb_image.h"

Vocksel::Texture::Texture(){

}

Vocksel::Texture::Texture(const char* path){
    loadTexture(path);
}

void Vocksel::Texture::createEmptyTexture(int width, int height) {
    if (texture_id_ != 0) {
        return;
    }

    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    height_ = height;
    width_ = width;

}

void Vocksel::Texture::loadTexture(const char *path) {
    if (texture_id_ != 0) {
        return;
    }

    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nr_channels;
    unsigned char* data = stbi_load(path, &width, &height, &nr_channels, 4);
    width_ = width;
    height_ = height;
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture " << path << std::endl;
    }
    stbi_image_free(data);
}

void Vocksel::Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, texture_id_);
}

void Vocksel::Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Vocksel::Texture::cleanUp() {
    if (texture_id_ != 0) {
        glDeleteTextures(1, &texture_id_);
        texture_id_ = 0;
        width_ = 0;
        height_ = 0;
    }
}

Vocksel::Texture::~Texture() {
    cleanUp();
}









