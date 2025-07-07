#include "Vocksel/sphere.h"

const std::string Vocksel::Sphere::MODEL_NAME = "sphere";

Vocksel::Sphere::Sphere(ModelManager &model_manager, ResourceManager &resource_manager)
    : model_manager_(model_manager), resource_manager_(resource_manager), position_(0.0f), radius_(1.0f), initialized_(false) {

}

std::unique_ptr<Vocksel::Sphere> Vocksel::Sphere::create(ModelManager &model_manager, ResourceManager &resource_manager, const glm::vec3 &pos, const float radius, const char *texture_name) {
    auto sphere = std::make_unique<Sphere>(model_manager, resource_manager);
    sphere->init(pos, radius, texture_name);
    return sphere;
}

void Vocksel::Sphere::init(glm::vec3 pos, float radius, const char *texture_name) {
    radius_ = radius;
    position_ = pos;
    texture_name_ = texture_name;

    initMesh(model_manager_);
    initialized_ = true;
}

void Vocksel::Sphere::initMesh(ModelManager &model_manager) {
    if (!model_manager.getModel(MODEL_NAME)) {
        model_manager.loadModel("assets/models/sphere.obj", "sphere");
    }
}


void Vocksel::Sphere::render(Shader &shader) {
    if (!initialized_) return;

    glDisable(GL_CULL_FACE);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position_));
    model = glm::scale(model, glm::vec3(radius_, radius_, radius_));

    Texture& texture = resource_manager_.getTexture(texture_name_);
    glActiveTexture(GL_TEXTURE0);
    texture.bind();
    shader.setInt("texture_diffuse", 0);


    if (auto* sphere_model = model_manager_.getModel(MODEL_NAME)) {
        sphere_model->setTransformMatrix(model);
        sphere_model->render(shader);
    }
}

void Vocksel::Sphere::cleanUp() {

}






