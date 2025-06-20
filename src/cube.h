#ifndef CUBE_H
#define CUBE_H
#include <glad/glad.h>
#include "glm/vec3.hpp"

class Cube {
public:
    Cube(glm::vec3 pos);
    ~Cube();
    void update(float deltaTime);
    void render(unsigned int shaderProgram);

private:
    glm::vec3 position_;
    float rotation_angle_;
    GLuint VAO_, VBO_, EBO_;

    // Cube vertices
    static const float vertices[24];
    static const unsigned int indices[36];

};

#endif //CUBE_H
