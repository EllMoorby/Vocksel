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
#include "Vocksel/Graphics/Mesh/model_instance.h"
#include "Vocksel/Resources/resource_manager.h"
#include "Vocksel/World/world.h"
#include "Vocksel/Resources/model_manager.h"
#include "Vocksel/Entities/player.h"
#include "constants.h"
#include "game.h"
#include "Vocksel/Entities/Debug/leg_creature.h"
#include "Vocksel/Graphics/shader.h"
#include "Vocksel/Graphics/camera.h"

namespace Vocksel {
    class Application {
    public:
        Application();
        ~Application();

        void run();

    private:
        void initWindow();
        void initGL();
        void initInput();

        void update(float delta_time);
        void render();
        void closeWindow();
        static void mouseCallback(GLFWwindow* window ,double xpos, double ypos);
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam);

        void cleanUp();
    private:
        bool in_mouse_mode_ = true;
        bool first_mouse_move_cond_ = true;
        double lastx_mouse_ = Constants::SCREEN_WIDTH/2, lasty_mouse_ = Constants::SCREEN_HEIGHT/2;
        float current_frame_;
        float last_frame_;
        float aspect_ratio_ = static_cast<float>(Constants::SCREEN_WIDTH) / static_cast<float>(Constants::SCREEN_HEIGHT);

        GLFWwindow* window_;
        Game game_;
    };
}
#endif //APPLICATION_H
