#include "Vocksel/Graphics/Mesh/model_instance.h"

#include "Vocksel/Core/engine_services.h"


std::unique_ptr<Vocksel::ModelInstance> Vocksel::ModelInstance::create(const glm::vec3 &pos, const char *model_name, const char *texture_name, const glm::vec3 &scale) {
    auto obj = std::make_unique<ModelInstance>();
    obj->init(pos, model_name, texture_name);
    obj->scale_ = scale;
    return obj;
}

void Vocksel::ModelInstance::init(glm::vec3 pos, const char *model_name, const char *texture_name) {
    position_ = pos;
    texture_name_ = texture_name;
    model_name_ = model_name;

    if (!EngineServices::models().getModel(model_name_)) {
        std::string model_path = std::string("assets/models/") + model_name_ + ".obj";
        EngineServices::models().loadModel(model_path, model_name_);
    }

    initialized_ = true;
}

void Vocksel::ModelInstance::render(Shader& shader) {
    if (!initialized_) return;

    glDisable(GL_CULL_FACE);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position_));
    model = glm::scale(model, scale_);
    model = glm::rotate(model, glm::radians(rotation_angle_), rotation_axis_);

    Texture& texture = EngineServices::resources().getTexture(texture_name_);
    glActiveTexture(GL_TEXTURE0);
    texture.bind();
    shader.setInt("texture_diffuse", 0);


    if (auto* sphere_model = EngineServices::models().getModel(model_name_)) {
        sphere_model->setTransformMatrix(model);
        sphere_model->render(shader);
    }
    glEnable(GL_CULL_FACE);
}

Vocksel::ModelInstance::ModelInstance(): position_(glm::vec3(0.f)) {

}

