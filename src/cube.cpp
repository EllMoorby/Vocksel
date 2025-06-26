#include "Vocksel/cube.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "Vocksel/camera.h"
#include "Vocksel/constants.h"
#include "Vocksel/shader.h"

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


unsigned int Vocksel::Cube::VAO_ = 0;
unsigned int Vocksel::Cube::VBO_ = 0;
unsigned int Vocksel::Cube::EBO_ = 0;


Vocksel::Cube::Cube()
    : position_(0.0f), color_(1.0f), rotation_angle_(0.0f), initialized_(false) {}

Vocksel::Cube Vocksel::Cube::create(const glm::vec3 &pos, const glm::vec3 &col) {
    Cube cube;
    cube.init(pos, col);
    return cube;
}


void Vocksel::Cube::initStaticBuffers() {
    if (VAO_ != 0) return; // already initialized

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Vocksel::Cube::cleanUpStaticBuffers() {
    if (VAO_ != 0) {
        glDeleteVertexArrays(1, &VAO_);
        glDeleteBuffers(1, &VBO_);
        glDeleteBuffers(1, &EBO_);
        VAO_ = 0;
        VBO_ = 0;
        EBO_ = 0;
    }
}



void Vocksel::Cube::init(glm::vec3 pos, glm::vec3 col) {
    color_ = col;
    position_ = pos;
    initialized_ = true;
}


void Vocksel::Cube::render(Shader& shader, const Camera& camera) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position_));
    model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(.3f, 1.0f, .5f));

    shader.setMat4("model", model);
    shader.setVec3("color", color_);

    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

Vocksel::Cube::Cube(Cube&& other) noexcept
    : position_(other.position_), color_(other.color_), rotation_angle_(other.rotation_angle_),  initialized_(other.initialized_) {

}

Vocksel::Cube& Vocksel::Cube::operator=(Cube&& other) noexcept {
    if (this != &other) {
        position_ = other.position_;
        color_ = other.color_;
        initialized_ = other.initialized_;
    }
    return *this;
}




