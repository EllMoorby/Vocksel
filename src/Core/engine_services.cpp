#include "Vocksel/Core/engine_services.h"

std::unique_ptr<Vocksel::ModelManager> Vocksel::EngineServices::model_manager_;
std::unique_ptr<Vocksel::ResourceManager> Vocksel::EngineServices::resource_manager_;
std::unique_ptr<Vocksel::InputManager> Vocksel::EngineServices::input_manager_;
std::unique_ptr<Vocksel::DebugRenderer> Vocksel::EngineServices::debug_renderer_;
std::unique_ptr<Vocksel::DebugGUI> Vocksel::EngineServices::debug_gui_;


void Vocksel::EngineServices::init(GLFWwindow *window) {
    debug_gui_ = std::make_unique<DebugGUI>();
    model_manager_ = std::make_unique<ModelManager>();
    resource_manager_ = std::make_unique<ResourceManager>();
    input_manager_ = std::make_unique<InputManager>();
    debug_renderer_ = std::make_unique<DebugRenderer>();

    debug_gui_->init(window);
    resource_manager_->init();
    input_manager_->init(window);
    debug_renderer_->init();
}

void Vocksel::EngineServices::updateFrameData(float dt, float aspect) {
    Frame::delta_time_ = dt;
    Frame::aspect_ratio_ = aspect;
    Frame::initialized_ = true;
}


Vocksel::EngineServices::~EngineServices() {
    cleanUp();
}
