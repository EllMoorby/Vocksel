#include "Vocksel/Core/application.h"
#include <format>
#define STB_IMAGE_IMPLEMENTATION

#if DEBUG
#include "tracy/Tracy.hpp"
#include "tracy/TracyOpenGL.hpp"
#endif

#include "stb/stb_image.h"
#include "Vocksel/Core/engine_services.h"



Vocksel::Application::Application() {
    initWindow();
    initGL();
    EngineServices::init(window_);
    initInput();

    game_.init();
}

void Vocksel::Application::initWindow() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

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
    glEnable(GL_DEPTH_CLAMP);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);


#if DEBUG
    TracyGpuContext;
#endif

    // https://learnopengl.com/In-Practice/Debugging

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);




}


// https://learnopengl.com/In-Practice/Debugging
void APIENTRY Vocksel::Application::glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}


void Vocksel::Application::initInput() {
    EngineServices::input().bindKey(GLFW_KEY_ESCAPE, [this] {
        closeWindow();
    });

    EngineServices::input().bindKey(GLFW_KEY_ENTER, [this] {
        EngineServices::input().setMouseMode(EngineServices::input().getMouseMode() == GLFW_CURSOR_DISABLED
            ? GLFW_CURSOR_NORMAL
            : GLFW_CURSOR_DISABLED);
    });
}


void Vocksel::Application::run() {
    glClearColor(game_.getClearColor().r, game_.getClearColor().g, game_.getClearColor().b, game_.getClearColor().a);

    float second_count = 0.0f;
    last_frame_ = glfwGetTime();
    while (!glfwWindowShouldClose(window_)) {
#if DEBUG
        ZoneScoped;
#endif
        float current_frame = glfwGetTime();
        float delta_time = current_frame - last_frame_;
        last_frame_ = current_frame;
        second_count += delta_time;

        // Work out FPS
        if (second_count >= 1.0f) {
            glfwSetWindowTitle(window_, std::format("Vocksel | {}", 1.0f/delta_time).c_str());
            second_count = 0;
        }



        update(delta_time);

        render();


        glfwSwapBuffers(window_);
        glfwPollEvents();

#if DEBUG
        FrameMark;
        TracyGpuCollect;
#endif
    }

}

void Vocksel::Application::update(float delta_time) {
#if DEBUG
    TracyGpuZone("Update")
#endif

    EngineServices::updateFrameData(delta_time,aspect_ratio_);




    EngineServices::input().update();




    game_.update(delta_time);




}


void Vocksel::Application::render() {
#if DEBUG
    ZoneScoped;
    TracyGpuZone("Render");
#endif

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    game_.render();
    EngineServices::debugGUI().render();


}


void Vocksel::Application::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->aspect_ratio_ = static_cast<float>(width) / static_cast<float>(height);
        glViewport(0, 0, width, height);
    }

}


void Vocksel::Application::mouseCallback(GLFWwindow *window_, double xpos, double ypos) {
    auto app = static_cast<Application*>(glfwGetWindowUserPointer(window_));
    if (EngineServices::input().isMouseFree()) return;

    if (app->first_mouse_move_cond_) {
        app->lastx_mouse_ = xpos;
        app->lasty_mouse_ = ypos;
        app->first_mouse_move_cond_ = false;
    }

    float xoffset = xpos - app->lastx_mouse_;
    float yoffset = app->lasty_mouse_ - ypos;
    app->lastx_mouse_ = xpos;
    app->lasty_mouse_ = ypos;

    app->game_.handleMouseInput(xoffset, yoffset);
}

void Vocksel::Application::closeWindow() {
    glfwSetWindowShouldClose(window_, GLFW_TRUE);
}


void Vocksel::Application::cleanUp() {
    glfwTerminate();
}

Vocksel::Application::~Application() {
    cleanUp();
}







