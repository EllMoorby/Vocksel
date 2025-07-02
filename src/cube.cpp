#include "Vocksel/cube.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "stb/stb_image.h"
#include "Vocksel/camera.h"
#include "Vocksel/shader.h"

const std::string Vocksel::Cube::MODEL_NAME = "cube";
GLuint Vocksel::Cube::texture_id_ = 0;

const float Vocksel::Cube::vertices_[] = {
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

const unsigned int Vocksel::Cube::indices_[] = {
    0, 1, 2, 2, 3, 0,    // Back
    4, 5, 6, 6, 7, 4,    // Front
    8, 9, 10, 10, 11, 8, // Left
    12, 13, 14, 14, 15, 12, // Right
    16, 17, 18, 18, 19, 16, // Bottom
    20, 21, 22, 22, 23, 20  // Top
};




Vocksel::Cube::Cube(ModelManager& model_manager)
    : model_manager_(model_manager), position_(0.0f), color_(1.0f), rotation_angle_(0.0f), initialized_(false) {}


std::unique_ptr<Vocksel::Cube> Vocksel::Cube::create(ModelManager& model_manager,const glm::vec3 &pos, const glm::vec3 &col) {
    auto cube = std::make_unique<Cube>(model_manager);
    cube->init(pos, col);
    return cube;
}

void Vocksel::Cube::init(glm::vec3 pos, glm::vec3 col) {
    color_ = col;
    position_ = pos;
    initialized_ = true;

    initMesh(model_manager_);

    initTexture("assets/textures/grass.png");
}


void Vocksel::Cube::initMesh(ModelManager& model_manager) {
    if (!model_manager.getModel(MODEL_NAME)) {
        model_manager.loadModel("assets/models/cube.obj", "cube");
    }
}

void Vocksel::Cube::initTexture(const char *texture) {
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
    unsigned char* data = stbi_load(texture, &width, &height, &nr_channels, 4);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture " << texture << std::endl;
    }
    stbi_image_free(data);
}


void Vocksel::Cube::cleanUp() {
    if (texture_id_ != 0) {
        glDeleteTextures(1, &texture_id_);
        texture_id_ = 0;
    }
}



void Vocksel::Cube::render(Shader& shader) {
    if (!initialized_) return;

    glDisable(GL_CULL_FACE);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position_));
    model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(.3f, 1.0f, .5f));

    shader.setVec3("color", color_);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
    shader.setInt("texture_diffuse", 0);

    if (auto* cube_model = model_manager_.getModel(MODEL_NAME)) {
        cube_model->setTransformMatrix(model);
        cube_model->render(shader);
    }

    glEnable(GL_CULL_FACE);

}





