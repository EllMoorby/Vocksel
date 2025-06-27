#include "Vocksel/application.h"
#include <format>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


Vocksel::Application::Application() {
    initWindow();
    initGL();

    shader_.init("assets/shaders/core/basic.vs.glsl", "assets/shaders/core/basic.fs.glsl");

    Chunk::initAtlas("assets/textures");
    const int worldSize = 10;
    for (int x = 0; x < worldSize; ++x)
        for (int y = 0; y < worldSize; ++y)
            for (int z = 0; z < worldSize; ++z)
                chunks_.emplace_back(glm::vec3(x, y, z) * (float)Chunk::kSize);


    cubes_.emplace_back(Cube::create(glm::vec3(-3.0f,.0f,0.f), glm::vec3(1.f,1.f,1.f)));



}

void Vocksel::Application::initWindow() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    window_ = glfwCreateWindow(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, "Vocksel", nullptr, nullptr);
    if (!window_) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");

    }
    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window_, mouseCallback);
    glfwSetWindowUserPointer(window_, this);


}

void Vocksel::Application::initGL() {
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glEnable(GL_DEPTH_TEST);
}

void Vocksel::Application::run() {
    float second_count = 0.0f;
    while (!glfwWindowShouldClose(window_)) {
        float current_frame = glfwGetTime();
        delta_time_ = current_frame - last_frame_;
        last_frame_ = current_frame;
        second_count += delta_time_;

        // Work out FPS
        if (second_count >= 1.0f) {
            glfwSetWindowTitle(window_, std::format("Vocksel | {}", 1.0f/delta_time_).c_str());
            second_count = 0;
        }
        processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_.use();

        // Calculate view matrix and set view and projection
        glm::mat4 view = camera_.getViewMatrix();
        glm::mat4 projection = camera_.getProjectionMatrix(
            static_cast<float>(Constants::SCREEN_WIDTH) /
            static_cast<float>(Constants::SCREEN_HEIGHT)
        );

        shader_.setMat4("view", view);
        shader_.setMat4("projection", projection);

        // Render cubes
        for (auto& cube : cubes_) {
            cube.render(shader_);
        }

        // Render chunks
        for (auto& chunk : chunks_) {
            chunk.render(shader_);
        }

        glfwSwapBuffers(window_);
        glfwPollEvents();

        // Show Errors (Should put in debug)
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL error: " << err << std::endl;
        }

    }
}

void Vocksel::Application::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}


void Vocksel::Application::mouseCallback(GLFWwindow *window_, double xpos, double ypos) {
    auto app = static_cast<Application*>(glfwGetWindowUserPointer(window_));
    if (app->first_mouse_move_cond_) {
        app->lastx_mouse_ = xpos;
        app->lasty_mouse_ = ypos;
        app->first_mouse_move_cond_ = false;
    }

    float xoffset = xpos - app->lastx_mouse_;
    float yoffset = app->lasty_mouse_ - ypos;
    app->lastx_mouse_ = xpos;
    app->lasty_mouse_ = ypos;

    const float kSsensitivity = 0.1f;
    xoffset *= kSsensitivity;
    yoffset *= kSsensitivity;

    app->yaw_camera_ += xoffset;
    app->pitch_camera_ += yoffset;


    if (app->pitch_camera_ > 89.0f) {
        app->pitch_camera_ = 89.0f;
    }
    if (app->pitch_camera_ < -89.0f) {
        app->pitch_camera_ = -89.0f;
    }


    glm::vec3 direction;
    direction.x = cos(glm::radians(app->yaw_camera_)) * cos(glm::radians(app->pitch_camera_));
    direction.y = sin(glm::radians(app->pitch_camera_));
    direction.z = sin(glm::radians(app->yaw_camera_)) * cos(glm::radians(app->pitch_camera_));
    app->camera_.front_ = glm::normalize(direction);


    app->camera_.recalculateVectors();
}

void Vocksel::Application::processInput() {
    float speed = Constants::CAMERA_SPEED * delta_time_;
    if (glfwGetKey(window_, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        speed = Constants::CAMERA_SPEED * delta_time_ * 2;
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window_, true);
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        camera_.moveForward(speed);
    }
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        camera_.moveBackward(speed);
    }
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
        camera_.moveLeft(speed);
    }
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
        camera_.moveRight(speed);
    }
    if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera_.moveUp(speed);
    }
    if (glfwGetKey(window_, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera_.moveDown(speed);
    }
}

void Vocksel::Application::cleanUp() {
    Cube::cleanUpMesh();
    glfwTerminate();
}

Vocksel::Application::~Application() {
    cleanUp();
}







