#ifndef ENGINE_SERVICES_H
#define ENGINE_SERVICES_H
#include "glad/glad.h"
#include "input_manager.h"
#include "Vocksel/Resources/model_manager.h"
#include "Vocksel/Resources/resource_manager.h"

namespace Vocksel {
    class EngineServices {
    public:
        ~EngineServices();
        static void init(GLFWwindow* window);

        static ModelManager& models() {
            if (!model_manager_) throw std::runtime_error("ModelManager not initialized");
            return *model_manager_;
        }
        static ResourceManager& resources() {
            if (!resource_manager_) throw std::runtime_error("ResourceManager not initialized");
            return *resource_manager_;
        }
        static InputManager& input() {
            if (!input_manager_) throw std::runtime_error("InputManager not initialized");
            return *input_manager_;
        }

        static void cleanUp() {
            input_manager_.reset();
            resource_manager_.reset();
            model_manager_.reset();
        }

    private:
        static std::unique_ptr<ModelManager> model_manager_;
        static std::unique_ptr<ResourceManager> resource_manager_;
        static std::unique_ptr<InputManager> input_manager_;
    };
}

#endif //ENGINE_SERVICES_H
