#include "Vocksel/Core/application.h"
#include <format>
#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Vocksel/Core/engine_services.h"


Vocksel::Application::Application() {
    initWindow();
    initGL();

    EngineServices::init(window_);

    initGUI();
    initInput();

    world_ = std::make_unique<World>();
    world_->init();
    player_.init(*world_);

    player_.setPosition(world_->getSpawnPosition());

    mesh_objects_.push_back(
    MeshObject::create(glm::vec3(-3.0f, .0f, 0.f), "teapot", "stone", glm::vec3(1.f))
);
    mesh_objects_.back()->setRotation(100);

    creature_ = std::make_unique<Creature>(glm::vec3(-3.f,8.f,0.f));

    debug_creature_ = std::make_unique<Debug::LegCreature>();



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
    glEnable(GL_DEPTH_CLAMP);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void Vocksel::Application::initGUI() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    ImGui_ImplGlfw_InitForOpenGL(window_, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init("#version 330 core");

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
    float second_count = 0.0f;
    last_frame_ = glfwGetTime();
    while (!glfwWindowShouldClose(window_)) {
        glClearColor(clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float current_frame = glfwGetTime();
        delta_time_ = current_frame - last_frame_;
        last_frame_ = current_frame;
        second_count += delta_time_;

        // Work out FPS
        if (second_count >= 1.0f) {
            glfwSetWindowTitle(window_, std::format("Vocksel | {}", 1.0f/delta_time_).c_str());
            second_count = 0;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        updateObjects();

        renderObjects();

        updateGUI();

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_);
        glfwPollEvents();

#ifdef DEBUG
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL error: " << err << std::endl;
        }
#endif
    }
}

void Vocksel::Application::updateObjects() {
    EngineServices::input().update(delta_time_);
    creature_->update(delta_time_);
    player_.update(EngineServices::input(), delta_time_);
    debug_creature_->update(delta_time_);
}


void Vocksel::Application::renderObjects() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto &basic_shader = EngineServices::resources().getShader("basic");
    basic_shader.use();
    basic_shader.setBool("showNormals", true);

    auto& camera = player_.getCamera();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix(aspect_ratio_);

    basic_shader.setMat4("view", view);
    basic_shader.setMat4("projection", projection);

    for (auto& obj : mesh_objects_) {
        obj->render(basic_shader);
    }
    creature_->render(basic_shader);
    debug_creature_->render(camera, aspect_ratio_);
    world_->render(basic_shader);

    auto* head = creature_->getHeadSegment().get();

    for (auto& leg : head->getLegs()) {
        glm::vec3 currentBase = head->getPosition();
        for (auto& segment : leg.segments_) {
            EngineServices::debug().drawLine(currentBase, segment.getTipPosition(), glm::vec3(1, 0, 0), camera, aspect_ratio_);
            currentBase = segment.getTipPosition();
        }
    }


    // Draw body segments legs
    for (auto& bodySegment : creature_->getBodySegments()) {
        for (auto& leg : bodySegment->getLegs()) {
            glm::vec3 currentBase = bodySegment->getPosition();

            for (auto& segment : leg.segments_) {
                EngineServices::debug().drawLine(currentBase, segment.getTipPosition(), glm::vec3(0, 1, 0),camera, aspect_ratio_); // Different color for body legs
                currentBase = segment.getTipPosition();
            }
        }
    }
}


void Vocksel::Application::updateGUI() {
    ImGui::Begin("Debug");
    ImGui::SeparatorText("Player");
    ImGui::Text("Player Position %.2f %.2f %.2f", player_.getPosition().x, player_.getPosition().y, player_.getPosition().z);
    ImGui::SeparatorText("Creature");
    ImGui::Text("Head Position %.2f %.2f %.2f", creature_->getPosition().x, creature_->getPosition().y, creature_->getPosition().z);

    ImGui::SeparatorText("Debug Creature");
    ImGui::SliderFloat("Pos X", &debug_creature_->position.x, -50.f, 50.f, "%.2f");
    ImGui::SliderFloat("Pos Y", &debug_creature_->position.y, -50.f, 50.f, "%.2f");
    ImGui::SliderFloat("Pos Z", &debug_creature_->position.z, -50.f, 50.f, "%.2f");

    ImGui::NewLine();

    ImGui::SliderFloat("Rest Pos X", &debug_creature_->offset.x, -50.f, 50.f, "%.2f");
    ImGui::SliderFloat("Rest Pos Y", &debug_creature_->offset.y, -50.f, 50.f, "%.2f");
    ImGui::SliderFloat("Rest Pos Z", &debug_creature_->offset.z, -50.f, 50.f, "%.2f");

    ImGui::NewLine();

    ImGui::InputFloat3("Add velocity to idx", new_vel, "%.2f");
    ImGui::InputInt("IDX", &vel_idx, 1, 100);
    if (ImGui::Button("Add velocity")) {
        debug_creature_->segment.getLegs().back().setVelocity(glm::vec3(new_vel[0], new_vel[1], new_vel[2]) ,vel_idx);
        std::cout << vel_idx << " " << new_vel<< std::endl;
    }
    ImGui::SameLine();
    if (ImGui::Button("Add To All")) {
        for (auto& vel : debug_creature_->segment.getLegs().back().velocities_) {
            vel = glm::vec3(new_vel[0], new_vel[1], new_vel[2]);
        }
    }

    ImGui::End();
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

    app->player_.handleMouseInput(xoffset, yoffset);
}

void Vocksel::Application::closeWindow() {
    glfwSetWindowShouldClose(window_, GLFW_TRUE);
}


void Vocksel::Application::cleanUp() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

Vocksel::Application::~Application() {
    cleanUp();
}







