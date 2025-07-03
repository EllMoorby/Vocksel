    #include "Vocksel/application.h"
    #include <format>
    #define STB_IMAGE_IMPLEMENTATION
    #include "stb/stb_image.h"
    #include <GLFW/glfw3.h>
    #include "imgui.h"
    #include "imgui_impl_glfw.h"
    #include "imgui_impl_opengl3.h"


    Vocksel::Application::Application() {
        initWindow();
        initGL();
        resources_.init();
        initGUI();
        initInput();

        world_ = std::make_unique<World>(resources_);
        world_->init();

        cubes_.emplace_back(Cube::create(model_manager_, resources_, glm::vec3(-3.0f, .0f, 0.f), glm::vec3(1.f, 1.f, 1.f), "stone"));
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
        //glEnable(GL_DEPTH_CLAMP);
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
        input_.init(window_);

        auto bind_movement = [this](int key, auto dirFunc) {
            input_.bindKey(key, [this, dirFunc](float dt) {
                float speed = Constants::CAMERA_SPEED * dt;
                if (input_.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
                    speed *= 2.0f;
                }
                dirFunc(speed);
            });
        };

        bind_movement(GLFW_KEY_W, [this](float speed) {
            camera_.moveForward(speed);
        });
        bind_movement(GLFW_KEY_S, [this](float speed) {
            camera_.moveBackward(speed);
        });
        bind_movement(GLFW_KEY_A, [this](float speed) {
            camera_.moveLeft(speed);
        });
        bind_movement(GLFW_KEY_D, [this](float speed) {
            camera_.moveRight(speed);
        });

        bind_movement(GLFW_KEY_SPACE, [this](float speed) {
            camera_.moveUp(speed);
        });
        bind_movement(GLFW_KEY_LEFT_CONTROL, [this](float speed) {
            camera_.moveDown(speed);
        });


        input_.bindKey(GLFW_KEY_ESCAPE, [this] {
            closeWindow();
        });

        input_.bindKey(GLFW_KEY_ENTER, [this] {
            input_.setMouseMode(input_.getMouseMode() == GLFW_CURSOR_DISABLED
                ? GLFW_CURSOR_NORMAL
                : GLFW_CURSOR_DISABLED);
        });

    }



    void Vocksel::Application::run() {
        float second_count = 0.0f;
        last_frame_ = glfwGetTime();
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

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            input_.update(delta_time_);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            auto &basic_shader = resources_.getShader("basic");
            basic_shader.use();
            basic_shader.setBool("showNormals", true);

            // Calculate view matrix and set view and projection
            glm::mat4 view = camera_.getViewMatrix();
            glm::mat4 projection = camera_.getProjectionMatrix(
                static_cast<float>(Constants::SCREEN_WIDTH) /
                static_cast<float>(Constants::SCREEN_HEIGHT)
            );

            basic_shader.setMat4("view", view);
            basic_shader.setMat4("projection", projection);

            // Render cubes
            for (auto& cube : cubes_) {
                cube->render(basic_shader);
            }

            world_->render(basic_shader);

            ImGui::Begin("Voxel Debug");
            ImGui::Text("FPS: %.1f", 1.0f / delta_time_);
            if (ImGui::Button("Regenerate")) {
                world_->generateWorld();
            }
            if (ImGui::SliderInt("Octaves",&world_->noise_num_octaves_, 1, 10)) {
                world_->generateWorld();
            }
            if (ImGui::SliderFloat("Frequency per octave",&world_->noise_freq_per_octave_, 0.01f, 10.0f)) {
                world_->generateWorld();
            }
            if (ImGui::SliderFloat("Amplitude per octave",&world_->noise_ampl_per_octave_, 0.01f, 10.0f)) {
                world_->generateWorld();
            }
            if (ImGui::SliderFloat("Noise Frequency",&world_->noise_frequency_, 0.0001f, 0.03f)) {
                world_->generateWorld();
            }

            ImGui::End();

            // Render ImGui
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window_);
            glfwPollEvents();

#ifdef Debug
            GLenum err;
            while ((err = glGetError()) != GL_NO_ERROR) {
                std::cerr << "OpenGL error: " << err << std::endl;
            }
#endif
        }
    }

    void Vocksel::Application::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }


    void Vocksel::Application::mouseCallback(GLFWwindow *window_, double xpos, double ypos) {
        auto app = static_cast<Application*>(glfwGetWindowUserPointer(window_));
        if (app->input_.isMouseFree()) return;

        if (app->first_mouse_move_cond_) {
            app->lastx_mouse_ = xpos;
            app->lasty_mouse_ = ypos;
            app->first_mouse_move_cond_ = false;
        }

        float xoffset = xpos - app->lastx_mouse_;
        float yoffset = app->lasty_mouse_ - ypos;
        app->lastx_mouse_ = xpos;
        app->lasty_mouse_ = ypos;

        xoffset *= Constants::CAMERA_SENS;
        yoffset *= Constants::CAMERA_SENS;

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

    void Vocksel::Application::closeWindow() {
        glfwSetWindowShouldClose(window_, GLFW_TRUE);
    }


    void Vocksel::Application::cleanUp() {
        Cube::cleanUp();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwTerminate();
    }

    Vocksel::Application::~Application() {
        cleanUp();
    }







