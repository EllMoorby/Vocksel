#include "Vocksel/Core/engine_services.h"

std::unique_ptr<Vocksel::ModelManager> Vocksel::EngineServices::model_manager_;
std::unique_ptr<Vocksel::ResourceManager> Vocksel::EngineServices::resource_manager_;
std::unique_ptr<Vocksel::InputManager> Vocksel::EngineServices::input_manager_;

void Vocksel::EngineServices::init(GLFWwindow *window) {
    model_manager_ = std::make_unique<ModelManager>();
    resource_manager_ = std::make_unique<ResourceManager>();
    input_manager_ = std::make_unique<InputManager>();
    resource_manager_->init();
    input_manager_->init(window);
}


Vocksel::EngineServices::~EngineServices() {
    cleanUp();
}
