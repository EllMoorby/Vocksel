#include "Vocksel/cube.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "stb/stb_image.h"
#include "Vocksel/camera.h"
#include "Vocksel/shader.h"

const std::string Vocksel::Cube::MODEL_NAME = "cube";

Vocksel::Cube::Cube(ModelManager& model_manager, ResourceManager& resource_manager)
    : model_manager_(model_manager), resource_manager_(resource_manager), position_(0.0f), color_(1.0f), rotation_angle_(0.0f), initialized_(false) {}


std::unique_ptr<Vocksel::Cube> Vocksel::Cube::create(ModelManager& model_manager, ResourceManager& resource_manager,const glm::vec3 &pos, const glm::vec3 &col, const char* texture_name) {
    auto cube = std::make_unique<Cube>(model_manager, resource_manager);
    cube->init(pos, col, texture_name);
    return cube;
}

void Vocksel::Cube::init(glm::vec3 pos, glm::vec3 col, const char* texture_name) {
    color_ = col;
    position_ = pos;
    texture_name_ = texture_name;

    initMesh(model_manager_);
    initialized_ = true;
}


void Vocksel::Cube::initMesh(ModelManager& model_manager) {
    if (!model_manager.getModel(MODEL_NAME)) {
        model_manager.loadModel("assets/models/cube.obj", "cube");
    }
}

void Vocksel::Cube::cleanUp() {

}

void Vocksel::Cube::render(Shader& shader) {
    if (!initialized_) return;

    glDisable(GL_CULL_FACE);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position_));
    model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(.3f, 1.0f, .5f));

    shader.setVec3("color", color_);

    Texture& texture = resource_manager_.getTexture(texture_name_);
    glActiveTexture(GL_TEXTURE0);
    texture.bind();
    shader.setInt("texture_diffuse", 0);


    if (auto* cube_model = model_manager_.getModel(MODEL_NAME)) {
        cube_model->setTransformMatrix(model);
        cube_model->render(shader);
    }

    glEnable(GL_CULL_FACE);
}





