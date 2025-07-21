#ifndef APPLICATION_H
#define APPLICATION_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "Vocksel/World/chunk.h"
#include "../Entities/Creature/creature.h"
#include "input_manager.h"
#include "Vocksel/Graphics/Mesh/mesh_object.h"
#include "Vocksel/Resources/resource_manager.h"
#include "Vocksel/World/world.h"
#include "Vocksel/Resources/model_manager.h"
#include "Vocksel/Entities/player.h"
#include "constants.h"
#include "Vocksel/Graphics/shader.h"
#include "Vocksel/Graphics/camera.h"

namespace Vocksel {
    class Application {
    public:
        Application();
        ~Application();

        void run();
        Player getPlayer() const {return player_;};
        float getAspectRatio() const {return aspect_ratio_;};


    private:
        void initWindow();
        void initGL();
        void initGUI();
        void initInput();

        void updateObjects();
        void updateGUI();
        void renderObjects();
        void closeWindow();
        static void mouseCallback(GLFWwindow* window ,double xpos, double ypos);
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

        void cleanUp();
    private:
        bool in_mouse_mode_ = true;
        bool first_mouse_move_cond_ = true;
        double lastx_mouse_ = Constants::SCREEN_WIDTH/2, lasty_mouse_ = Constants::SCREEN_HEIGHT/2;
        glm::vec4 clear_color_ = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
        float current_frame_;
        float delta_time_;
        float last_frame_;
        float aspect_ratio_ = static_cast<float>(Constants::SCREEN_WIDTH) / static_cast<float>(Constants::SCREEN_HEIGHT);

        GLFWwindow* window_;
        Player player_;
        std::vector<std::unique_ptr<MeshObject>> mesh_objects_;
        std::unique_ptr<Creature> creature_;
        std::unique_ptr<World> world_;
    };
}
#endif //APPLICATION_H
