#include "Application.h"



Application::Application(const std::string& title , int width , int height) : title(title), WIN_WIDTH(width), WIN_HEIGHT(height), window(nullptr), lastX(width/2.0f), lastY(height/2.0f) { 
    std::cout << "Application started with title: " << title 
              << ", width: " << width 
              << ", height: " << height << std::endl; 
            init();
}

Application* Application::getApplicationPtr() { 
    return static_cast<Application*>(glfwGetWindowUserPointer(this->window));
}

bool Application::init() { 


    //Initialize GLFW and create a window
    std::cout << "Initializing application..." << std::endl; 

    initGLFW();

    glfwMakeContextCurrent(this->window); 


    //Load OpenGL functions using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { 
        std::cerr << "Failed to initialize GLAD!" << std::endl; 
        return false; 
    } 

    devCamera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    initShader();

    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT); // Set the viewport to the window size
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the clear color to a light gray



    return true; 
}

bool Application::initGLFW() {
    if (!glfwInit()) { 
        std::cerr << "Failed to initialize GLFW!" << std::endl; 
        return false; 
    } 

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);

    //create window
    this->window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, title.c_str(), nullptr, nullptr); 

    if(this->window == nullptr) { 
        std::cerr << "Failed to create GLFW window!" << std::endl; 
        glfwTerminate(); 
        return false; 
    }
    std::cout<< "GLFW initialized and window created successfully!" << std::endl;
    return true; 
}

bool Application::initShader() { 
    try {
        _mainShader = std::make_shared<Shader>("./shaders/vertex.vert", "./shaders/fragment.frag");
        std::cout << "Shader initialized successfully!" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize shader: " << e.what() << std::endl;
        return false;
    }

}
 
void Application::processInput() { 
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { 
        glfwSetWindowShouldClose(this->window, true); 
    } 
    if(glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) { 
        devCamera->handleCameraMovement(FORWARD, 0.1f);
    }
}

void Application::render(){ 
    std::cout << "Rendering..." << std::endl;
}





Application::~Application() { 
    std::cout << "Application stopped!" << std::endl; 
}


void Application::run() { 
    std::cout << "Running application..." << std::endl; 
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    auto vbo = createVertexBuffer(GL_STATIC_DRAW);
    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f, // Bottom left
         0.5f, -0.5f, 0.0f, // Bottom right
         0.0f,  0.5f, 0.0f,  // Top
         -0.5f, 0.7f, 0.0f
    };
    vbo.setData(vertices);

    GLVertexArray vao;
    vao.addVertexBuffer(vbo, 0, 3, GL_FLOAT); // Add vertex buffer to VAO with index 0




    while(!glfwWindowShouldClose(this->window)) { 
        // glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth


        _mainShader->use();
        vao.drawArrays(GL_TRIANGLES, 0, 3); // Draw the triangle using the VAO


        glBindVertexArray(0);
        glfwPollEvents();
        glfwSwapBuffers(window); // Swap the front and back buffers
    } 
}


void Application::updateProjectionMatrix() { 
    float aspectRatio = static_cast<float>(WIN_WIDTH) / static_cast<float>(WIN_HEIGHT);
    float halfHeight = WIN_HEIGHT / 2.0f;
    float halfWidth = aspectRatio * halfHeight;
    projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f); // Orthographic projection matrix
    if(_mainShader->ID != 0) {
        _mainShader->use();
        _mainShader->setMat4("projection", projection); // Set the projection matrix in the shader
    } else {
        std::cerr << "Error updating Projection Matrix: Shader not initialized!" << std::endl;
    }   
}


void Application::updateViewMatrix() { 
    // Update the view matrix if needed
    // This can be used to set camera position, orientation, etc.
    // For now, we will just print the current projection matrix
    glm::mat4 view = glm::mat4(1.0f); // Identity matrix for view
    if(_mainShader->ID != 0) {
        _mainShader->use();
        _mainShader->setMat4("view", view); // Set the view matrix in the shader
    } else {
        std::cerr << "Error updating View Matrix: Shader not initialized!" << std::endl;
    }   
}




void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
            Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
            if(app) {
                app->WIN_WIDTH = width;
                app->WIN_HEIGHT = height;
            }
        }   


