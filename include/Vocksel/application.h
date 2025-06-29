#ifndef APPLICATION_H
#define APPLICATION_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "chunk.h"
#include "world.h"
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
        void processInput();
        static void mouseCallback(GLFWwindow* window ,double xpos, double ypos);
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        void cleanUp();
    private:
        bool first_mouse_move_cond_ = true;
        double lastx_mouse_ = Constants::SCREEN_WIDTH/2, lasty_mouse_ = Constants::SCREEN_HEIGHT/2;
        float yaw_camera_ = 90.0f, pitch_camera_;
        float current_frame_;
        float delta_time_;
        float last_frame_;

        GLFWwindow* window_;
        Camera camera_;
        Shader shader_;
        std::vector<Cube> cubes_;
        World world_;
};
}
#endif //APPLICATION_H
