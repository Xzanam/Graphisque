#include "Application.h"



Application::Application(const std::string& title , int width , int height) : title(title), 
WIN_WIDTH(width), WIN_HEIGHT(height), window(nullptr), _lastX(width/2.0f), _lastY(height/2.0f), _isCursorHidden(false) { 
    std::cout << "Application started with title: " << title 
              << ", width: " << width 
              << ", height: " << height << std::endl; 
}

Application* Application::getApplicationPtr(GLFWwindow* window) { 
    return static_cast<Application*>(glfwGetWindowUserPointer(window));
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

    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering
    initShader();

    setupCallbacks();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    _isCursorHidden = true;
    


    devCamera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    initGrid3D();

    // updateProjectionMatrix();
    // updateViewMatrix();


    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT); // Set the viewport to the window size
    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the clear color to a light gray
    _mainShader->use();
    _mainShader->setMat4("model", glm::mat4(1.0f));


    // cube = new Cube();
    // cube->setShader(_mainShader);
    // cube->setPosition(glm::vec3(0.0f));


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

    glfwSetWindowUserPointer(window , this);

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

void Application::initGrid3D() { 
    GridConfig config;
    grid3D = std::make_shared<Grid3D>(config, _mainShader);
}
 
void Application::processInput(float deltaTime) { 
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { 
        glfwSetWindowShouldClose(this->window, true); 
    }
    if(glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) { 
        devCamera->handleCameraMovement(FORWARD, deltaTime);

    } 
    if(glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) { 
        devCamera->handleCameraMovement(BACKWARD, deltaTime);
    }
    if(glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) { 
        devCamera->handleCameraMovement(LEFT, deltaTime);
    }
    if(glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) { 
        devCamera->handleCameraMovement(RIGHT, deltaTime);
    }   
    if(glfwGetKey(this->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) { 
        devCamera->handleCameraMovement(DOWN, deltaTime);
    }   
    if(glfwGetKey(this->window,GLFW_KEY_SPACE ) == GLFW_PRESS) { 
        devCamera->handleCameraMovement(UP, deltaTime);
    }   
    if(glfwGetKey(this->window,GLFW_KEY_H) == GLFW_PRESS) { 



    }

}


void Application::render(float deltaTime){ 
    // std::cout << "Rendering..." << std::endl;    
    // grid3D->render(*devCamera);

    // cube->render(*devCamera);

}



void Application::run() { 
    std::cout << "Running application..." << std::endl; 
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

 
    lastFrame = static_cast<float>(glfwGetTime());

    float vertices[] = { 
        -1.0f, 0.0f,0.0f, 
        1.0f, 0.0f,0.0f, 
        0.0f, 1.0f, 0.0f
    };
    
    float pyramidVertices[] = { 
        0.0f, 2.0f, 0.0f, //topvertex
        -1.0f, 0.0f, -1.0f,  // 
        -1.0f, 0.0f, 1.0f, 
        1.0f, 0.0f, 1.0f, 
        1.0f, 0.0f, -1.0f
    };

    unsigned int indices[] = { 
        //base of the pyramid
        1, 2, 3,
        1, 4, 3, 

        //sides
        0, 1, 2, 
        0, 2, 3, 
        0, 4, 4, 
        0, 1, 4,


    };

    VertexBuffer buffer;
    buffer = createVertexBuffer();
    buffer.setData(pyramidVertices, sizeof(pyramidVertices));
    VerteXArray arrayobj;
    arrayobj.addVertexBuffer(buffer, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(WIN_WIDTH) / (float) WIN_HEIGHT, 0.1f, 100.0f);



    _mainShader->use();
    _mainShader->setMat4("model", model);
    _mainShader->setMat4("projection", projection);

    Cylinder cylinder(0.5f, 5.0f,20);



    // Circle myCircle(10.0f);

    glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);



    while(!glfwWindowShouldClose(this->window)) { 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth

        //Process input 
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame; 
        processInput(deltaTime);

        render(deltaTime);

        _mainShader->use();
        _mainShader->setMat4("view", devCamera->getViewMatrix());

        cylinder.draw();
        




        // myCircle.render(*_mainShader, devCamera);
        // _mainShader->use();
        // arrayobj.drawArrays(GL_TRIANGLES, 0, 3);

        // _mainShader->use();
        // _mainShader->setMat4("view", devCamera->getViewMatrix());
        // arrayobj.drawArrays(GL_TRIANGLES, 0, 3);


        glfwSwapBuffers(window); // Swap the front and back buffers
        glfwPollEvents();
    } 
}




Application::~Application() { 
    std::cout << "Application stopped!" << std::endl; 
}


void Application::updateProjectionMatrix() { 
    float aspectRatio = static_cast<float>(WIN_WIDTH) / static_cast<float>(WIN_HEIGHT);
    float halfHeight = WIN_HEIGHT / 2.0f;
    float halfWidth = aspectRatio * halfHeight;
    projection = glm::perspective(glm::radians(45.0f), (float)WIN_WIDTH / (float) WIN_HEIGHT, 0.1f, 100.0f);
    devCamera->setProjectionMatrix(projection);
    if(_mainShader->ID != 0) {
        _mainShader->use();
        _mainShader->setMat4("projection", projection); // Set the projection matrix in the shader
        // _mainShader->setMat4("model" ,glm::mat4(1.0f));

    } else {
        std::cerr << "Error updating Projection Matrix: Shader not initialized!" << std::endl;
    }   
}


void Application::updateViewMatrix() { 
    // Update the view matrix if needed
    // This can be used to set camera position, orientation, etc.
    // For now, we will just print the current projection matrix
    glm::mat4 view = devCamera->getViewMatrix(); // Identity matrix for view
    if(_mainShader->ID != 0) {
        _mainShader->use();
        _mainShader->setMat4("view", view); // Set the view matrix in the shader
    } else {
        std::cerr << "Error updating View Matrix: Shader not initialized!" << std::endl;
    }   
}


void Application::setupCallbacks(){ 
    glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
    glfwSetCursorPosCallback(this->window, cursor_pos_callback);
    glfwSetKeyCallback(this->window, key_callback);
}




void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
            Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
            if(app) {
                app->WIN_WIDTH = width;
                app->WIN_HEIGHT = height;
            }
        }   

void Application::cursor_pos_callback(GLFWwindow* window, double xPosIn, double yPosIn) { 
    Application * app = Application::getApplicationPtr(window);
    if(!app ){ 
        std::cerr<< "(cursor_pos_callback) Failed getting window:" << std::endl;
        return;
    }
    float xpos = static_cast<float>(xPosIn);
    float ypos = static_cast<float>(yPosIn);
    if(app->_firstMouse) { 
        app->_lastX = xpos;
        app-> _lastY = ypos;
        app->_firstMouse = false;
    }

    float xOffset = xpos - app->_lastX;
    float yOffset = app->_lastY - ypos;

    app->_lastX = xpos;
    app->_lastY  = ypos;

    app->devCamera->handleMouseMovement(xOffset, yOffset,GL_TRUE);

}


void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){ 
    Application* app = getApplicationPtr(window);
    if  (key == GLFW_KEY_H && action == GLFW_PRESS) { 
        if(app-> _isCursorHidden){

            glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
            app-> _isCursorHidden= false;
        }
        else{
            glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
            app-> _isCursorHidden=true;
        }
    }
}