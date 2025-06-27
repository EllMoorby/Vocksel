#include "Vocksel/cube.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "stb/stb_image.h"
#include "Vocksel/camera.h"
#include "Vocksel/constants.h"
#include "Vocksel/shader.h"

std::unique_ptr<Vocksel::StaticMesh> Vocksel::Cube::mesh_ = nullptr;
GLuint Vocksel::Cube::texture_id = 0;


const float Vocksel::Cube::vertices[] = {
    // Positions          // Texture Coords
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

    // Front face
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f
};

const unsigned int Vocksel::Cube::indices[] = {
    0, 1, 2, 2, 3, 0,    // Back
    4, 5, 6, 6, 7, 4,    // Front
    8, 9, 10, 10, 11, 8, // Left
    12, 13, 14, 14, 15, 12, // Right
    16, 17, 18, 18, 19, 16, // Bottom
    20, 21, 22, 22, 23, 20  // Top
};




Vocksel::Cube::Cube()
    : position_(0.0f), color_(1.0f), rotation_angle_(0.0f), initialized_(false) {}

Vocksel::Cube Vocksel::Cube::create(const glm::vec3 &pos, const glm::vec3 &col) {
    Cube cube;
    cube.init(pos, col);
    return cube;
}

void Vocksel::Cube::init(glm::vec3 pos, glm::vec3 col) {
    color_ = col;
    position_ = pos;
    initialized_ = true;

    initMesh();
    initTexture("assets/textures/grass.jpg");
}


void Vocksel::Cube::initMesh() {
    if (mesh_) return;
    mesh_ = std::make_unique<StaticMesh>(
        vertices,
        sizeof(vertices) / sizeof(float),
        indices,
        sizeof(indices) / sizeof(unsigned int),
        5
    );
}

void Vocksel::Cube::initTexture(const char *texture) {
    if (texture_id != 0) {
        return;
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(texture, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture " << texture << std::endl;
    }
    stbi_image_free(data);
}


void Vocksel::Cube::cleanUpMesh() {
    mesh_.reset();
}



void Vocksel::Cube::render(Shader& shader, const Camera& camera) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position_));
    model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(.3f, 1.0f, .5f));

    shader.setMat4("model", model);
    shader.setVec3("color", color_);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    shader.setInt("texture_diffuse", 0);  // Add this line

    mesh_->bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    mesh_->unbind();
}





