#include "Application.h"


float aspectRatio = static_cast<float>(WinWidth) / static_cast<float>(WinHeight); // Initialize aspect ratioh

float debounceDelay = 0.1f; //for keypresses, in processInput() ;
std::chrono::time_point<std::chrono::high_resolution_clock> lastKeyTime = std::chrono::high_resolution_clock::now();

float panSpeed = 5.0f;

Application::Application(const std::string& title , int width , int height) : title(title), WIN_WIDTH(width), WIN_HEIGHT(height), window(nullptr), lastX(width/2.0f), lastY(height/2.0f), isPanning(false) { 
    std::cout << "Application started with title: " << title 
              << ", width: " << width 
              << ", height: " << height << std::endl; 
            grid = new Grid(1.0f, 500, 500, 2.0f); // Initialize grid with cell size, lines, and line width
            init();
}


Application* Application::getApplicationPtr(GLFWwindow* window) { 
    return static_cast<Application*>(glfwGetWindowUserPointer(window));
}


bool Application::init() { 


    //Initialize GLFW and create a window
    std::cout << "Initializing application..." << std::endl; 
    initGLFW();

    glfwMakeContextCurrent(this->window); 

    // glfwSwapInterval(0); // Disable vsync for smoother rendering


    //Load OpenGL functions using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { 
        std::cerr << "Failed to initialize GLAD!" << std::endl; 
        return false; 
    } 

    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT); // Set the viewport to the window size
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the clear color to a light gray

    left = targetLeft = -1.0f * aspectRatio;
    right = targetRight = 1.0f * aspectRatio;
    bottom = targetBottom = -1.0f;
    top = targetTop =1.0f;

    bool result = initShader();
    if(!result) 
        std::cerr<< "Shader Initialization Failed" << std::endl;
    else { 
        std::cout<< "Shaders Initialized" <<std::endl;
        updateProjectionMatrix();
    }

    glfwSetWindowUserPointer(window, this);


    grid->initialize(); // Initialize grid buffers and geometry
    grid->setShader(*gridShader); // Set the shader for the grid


          // Set initial viewport
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        grid->setViewport(static_cast<float>(width), static_cast<float>(height));
        
        // Configure grid appearance
        grid->setMajorGridColor(0.7f, 0.7f, 0.7f, 1.0f);
        grid->setMinorGridColor(0.5f, 0.3f, 0.5f, 0.8f);
        grid->setMajorGridInterval(10);
        grid->setZoomLimits(0.05f, 50.0f);
        
        // Configure smooth transitions
        grid->setSmoothTransitions(true);
        grid->setPanSmoothness(10.0f);
        grid->setZoomSmoothness(8.0f);
        
        // Set initial view
        grid->fitToView(false);

       
    // // Configure appearance
    // graph.setMajorGridColor(1.0f, 1.0f, 1.0f, 1.0f);  // White major lines
    // graph.setMinorGridColor(0.5f, 0.5f, 0.5f, 0.8f);  // Gray minor lines

    return true; 
}

void Application::updateProjectionMatrix() { 

    projectionMatrix = glm::ortho(left,right,bottom, top);
    glm::mat4 model = glm::mat4(1.0f);

    if(drawShader){ 
        drawShader->use();
        drawShader->setMat4("projection", projectionMatrix);
        drawShader->setMat4("model", model);
    }

    // if(gridShader) { 
    //     gridShader->use();
    //     gridShader->setMat4("projection", projectionMatrix);
    // }
    
}

bool Application::initGLFW() {
    if (!glfwInit()) { 
        std::cerr << "Failed to initialize GLFW!" << std::endl; 
        return false; 
    } 

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
     glfwWindowHint(GLFW_SAMPLES, 4); // Enable multisampling for smoother lines

    //create window
    this->window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, title.c_str(), nullptr, nullptr); 

    if(this->window == nullptr) { 
        std::cerr << "Failed to create GLFW window!" << std::endl; 
        glfwTerminate(); 
        return false; 
    }
    std::cout<< "GLFW initialized and window created successfully!" << std::endl;

    setUpCallbacks();

    return true; 

}
 


bool Application::initShader(){ 
    gridShader = new Shader("./shaders/grid.vert", "./shaders/grid.frag");
    drawShader = new Shader("./shaders/vertex.vert", "./shaders/fragment.frag");

    if(!gridShader || !drawShader) 
        return false;
    return true;
}

void Application::render(){ 
    std::cout << "Rendering..." << std::endl;
}

Application::~Application() { 
//     delete gridShader; 
// delete drawShader;
    std::cout << "Application stopped!" << std::endl; 
}


void Application::run() { 
    std::cout << "Running application..." << std::endl; 
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    auto vbo = createVertexBuffer(GL_STATIC_DRAW);
    std::vector<float> vertices = {
        -10.0f, -10.0f, 0.0f, // Bottom left
         10.0f, -10.0f, 0.0f, // Bottom right
         0.0f,  10.0f, 0.0f,  // Top
    };
    vbo.setData(vertices);

    GLVertexArray vao;
    vao.addVertexBuffer(vbo, 0, 3, GL_FLOAT); // Add vertex buffer to VAO with index 0


  
    float lastFrame = glfwGetTime();
    double lastFPSTime = lastFrame;
    int nbFrames = 0; // Frame counter for FPS calculation

    while(!glfwWindowShouldClose(this->window)) { 
        float currentFrame = glfwGetTime();

        float deltaTime  =currentFrame - lastFrame;
        lastFrame = currentFrame;
        nbFrames++;
        if(currentFrame - lastFPSTime > 1.0f && showFPS == true) { 
            std::cout<< nbFrames<< "FPS\n";
            nbFrames = 0;
            lastFPSTime += 1.0f;
        }
        // glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth
        // processInput(deltaTime);

  
     

        grid->update(deltaTime);
        

        grid->render();

        glm::mat4 mvp = grid->getMVPMatrix();
        drawShader->use();
        drawShader->setMat4("mvp", mvp);

        vao.drawArrays(GL_TRIANGLES, 0, 4); // Draw the triangle using the VAO
        glBindVertexArray(0); // Unbind the VAO

        glfwPollEvents();
        glfwSwapBuffers(window); // Swap the front and back buffers
    } 
}


void Application::processInput(float deltaTime) { 
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { 
        glfwSetWindowShouldClose(this->window, true); 
    } 

    auto now = std::chrono::high_resolution_clock::now();
    float elapsed = std::chrono::duration<float>(now - lastKeyTime).count();

    float moveAmount = panSpeed * deltaTime;
    if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS && elapsed > debounceDelay){ 
        // std::cout<< "Right Arrow Pressed" << std::endl;
        panGrid(-10.0f * deltaTime, 0.0f);
        lastKeyTime = now;
    }
    if(glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS && elapsed > debounceDelay) { 
        // std::cout<< "Left Arrow Pressed" << std::endl;
        panGrid(10.0f *deltaTime, 0.0f);
        lastKeyTime = now;
    }
}


void Application::panGrid(float deltaX, float deltaY) { 
    targetLeft  += deltaX;
    targetRight += deltaX;
    targetBottom += deltaY;
    targetTop += deltaY;
    isPanning = true;
}


float Application::expDecayLerp(float current, float target, float decay, float deltaTime) { 
    return target + (current - target ) * exp(-decay * deltaTime);

}

float Application::lerpPan(float start,float end, float t){ 
    return start + t * (end - start);
}


void Application::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) { 
    Application *app = getApplicationPtr(window);
    if(app && app->grid) { 
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        app->grid->handleMouseButton(button, action,mods,  xPos, yPos);
    }
}


void Application::cursor_position_callback(GLFWwindow* window,double xPos, double yPos)
{ 
    Application *app = getApplicationPtr(window);
    if(app && app->grid) { 
        app->grid->handleMouseMove(xPos, yPos);
    }

}

void Application::frameBufferSize_callback(GLFWwindow* window,int  width, int height) { 
    glViewport(0,  0,width, height) ;
    Application *app = getApplicationPtr(window);
    if(app ) { 
        app->WIN_WIDTH = width;
        app->WIN_HEIGHT = height;
    }
    if(app && app->grid) { 
        app->grid->setViewport(static_cast<float>(width), static_cast<float>(height));
    }

}

void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) { 
    Application *app = getApplicationPtr(window);
    // std::cout<< "Scroll callback: xoffset = " << xoffset << ", yoffset = " << yoffset << std::endl;
    if(app && app-> grid) { 
        app->grid->handleMouseScroll(xoffset, yoffset);
    }
}


void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application *app = getApplicationPtr(window);
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, true);
                break;
            case GLFW_KEY_R:
                if (app->grid && app) app->grid->resetView(true);
                break;
            case GLFW_KEY_F:
                if (app->grid && app) app->grid->fitToView(true);
                break;
            case GLFW_KEY_T:
                if (app->grid && app) {
                    app->grid->setSmoothTransitions(!app->grid->isSmoothTransitions());
                    std::cout << "Smooth transitions: " << (app->grid->isSmoothTransitions() ? "ON" : "OFF") << std::endl;
                }
                break;
        }
    }
}


void Application::setUpCallbacks() { 
    glfwSetFramebufferSizeCallback(this->window, frameBufferSize_callback);
    glfwSetCursorPosCallback(this->window,cursor_position_callback);
    glfwSetScrollCallback(this->window, scroll_callback);
    glfwSetKeyCallback(this->window, key_callback);
    glfwSetMouseButtonCallback(this->window, mouse_button_callback);
}





