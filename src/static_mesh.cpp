#include "Vocksel/static_mesh.h"


Vocksel::StaticMesh::StaticMesh(const float* vertices, int vertex_count, const unsigned int* indices, int index_count, int vertex_stride): index_count_(index_count) {
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(float) * vertex_stride, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //Unsure if needed
    glBindVertexArray(0);
}


Vocksel::StaticMesh::~StaticMesh() {
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
    glDeleteBuffers(1, &EBO_);
}

void Vocksel::StaticMesh::bind() {
    glBindVertexArray(VAO_);
}

void Vocksel::StaticMesh::unbind() {
    glBindVertexArray(0);
}

int Vocksel::StaticMesh::getIndexCount() const {
    return index_count_;
}

