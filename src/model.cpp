#include "../include/Vocksel/model.h"


Vocksel::Model::Model() {

}

void Vocksel::Model::addMesh(std::unique_ptr<StaticMesh> mesh) {
    meshes_.push_back(std::move(mesh));
}


bool Vocksel::Model::loadFromFile(std::string path) {
    //TODO: fill in
    return false;
}

void Vocksel::Model::render(Shader &shader) {

    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    if (currentProgram != shader.ID_) {
        std::cerr << "ERROR: Wrong shader bound! Expected " << shader.ID_
                  << " but found " << currentProgram << std::endl;
        return;
    }

    shader.setMat4("model", transform_);

    for (const auto& mesh : meshes_) {

        mesh->bind();

        glDrawElements(GL_TRIANGLES, mesh->getIndexCount(), GL_UNSIGNED_INT, 0);
        mesh->unbind();
    }
}

void Vocksel::Model::setTransformMatrix(const glm::mat4 &transform) {
    transform_ = transform;
}

void Vocksel::Model::cleanUp() {
    meshes_.clear();
}

Vocksel::Model::~Model() {
    cleanUp();
}





