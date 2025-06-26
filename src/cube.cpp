#include "Vocksel/cube.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "Vocksel/camera.h"
#include "Vocksel/constants.h"
#include "Vocksel/shader.h"

std::unique_ptr<Vocksel::StaticMesh> Vocksel::Cube::mesh_ = nullptr;

const float Vocksel::Cube::vertices[24] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f
};

const unsigned int Vocksel::Cube::indices[36] = {
    0, 1, 2, 2, 3, 0,    // back face
    4, 5, 6, 6, 7, 4,    // front face
    3, 2, 6, 6, 7, 3,    // top face
    0, 1, 5, 5, 4, 0,    // bottom face
    0, 3, 7, 7, 4, 0,    // left face
    1, 2, 6, 6, 5, 1     // right face
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
}


void Vocksel::Cube::initMesh() {
    if (mesh_) return;
    mesh_ = std::make_unique<StaticMesh>(vertices, 24, indices,36, 3);
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

    mesh_->bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    mesh_->unbind();
}





