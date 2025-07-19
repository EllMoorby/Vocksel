#ifndef APPLICATION_H
#define APPLICATION_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "chunk.h"
#include "creature.h"
#include "input_manager.h"
#include "mesh_object.h"
#include "resource_manager.h"
#include "world.h"
#include "model_manager.h"
#include "player.h"
#include "Vocksel/constants.h"
#include "Vocksel/shader.h"
#include "Vocksel/camera.h"

namespace Vocksel {
    class Application {
    public:
        Application();
        ~Application();

        void run();

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
