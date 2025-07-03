#ifndef APPLICATION_H
#define APPLICATION_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "chunk.h"
#include "input_manager.h"
#include "resource_manager.h"
#include "world.h"
#include "model_manager.h"
#include "player.h"
#include "Vocksel/constants.h"
#include "Vocksel/cube.h"
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
        void closeWindow();
        static void mouseCallback(GLFWwindow* window ,double xpos, double ypos);
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        void cleanUp();
    private:
        bool in_mouse_mode_ = true;
        bool first_mouse_move_cond_ = true;
        double lastx_mouse_ = Constants::SCREEN_WIDTH/2, lasty_mouse_ = Constants::SCREEN_HEIGHT/2;
        float current_frame_;
        float delta_time_;
        float last_frame_;

        GLFWwindow* window_;
        InputManager input_;
        ResourceManager resources_;
        ModelManager model_manager_;
        Player player_;
        std::vector<std::unique_ptr<Cube>> cubes_;
        std::unique_ptr<World> world_;
};
}
#endif //APPLICATION_H
