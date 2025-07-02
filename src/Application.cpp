#include "Application.h"


float aspectRatio = static_cast<float>(WinWidth) / static_cast<float>(WinHeight); // Initialize aspect ratioh

Application::Application(const std::string& title , int width , int height) : title(title), WIN_WIDTH(width), WIN_HEIGHT(height), window(nullptr), lastX(width/2.0f), lastY(height/2.0f) { 
    std::cout << "Application started with title: " << title 
              << ", width: " << width 
              << ", height: " << height << std::endl; 
              graph = Grid(0.2f, 100, 100, 1.0f);
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


    //Load OpenGL functions using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { 
        std::cerr << "Failed to initialize GLAD!" << std::endl; 
        return false; 
    } 

    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT); // Set the viewport to the window size
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set the clear color to a light gray

    bool result = initShader();
    if(!result) 
        std::cerr<< "Shader Initialization Failed" << std::endl;
    else { 
        std::cout<< "Shaders Initialized" <<std::endl;
        setUpProjectionMatrix();
    }

    graph = Grid(0.5f, 100, 100, 0.1f);
    graph.setShader(*gridShader);
    graph.initGrid();

       
    // Configure appearance
    graph.setMajorGridColor(1.0f, 1.0f, 1.0f, 1.0f);  // White major lines
    graph.setMinorGridColor(0.5f, 0.5f, 0.5f, 0.8f);  // Gray minor lines

    return true; 
}

void Application::setUpProjectionMatrix() { 
    left = -1.0f * aspectRatio;
    right = 1.0f * aspectRatio;
    bottom = -1.0f;
    top =1.0f;
    projectionMatrix = glm::ortho(left,right,bottom, top);
    glm::mat4 model = glm::mat4(1.0f);

    drawShader->use();
    drawShader->setMat4("projection", projectionMatrix);
    drawShader->setMat4("model", model);

    gridShader->use();
    gridShader->setMat4("projection", projectionMatrix);
    gridShader->setMat4("model", model);
    
}

bool Application::initGLFW() {
    if (!glfwInit()) { 
        std::cerr << "Failed to initialize GLFW!" << std::endl; 
        return false; 
    } 

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

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
 
void Application::processInput() { 
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { 
        glfwSetWindowShouldClose(this->window, true); 
    } 
    if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS){ 
        std::cout<< "Right Arrow Pressed" << std::endl;
        right += 5;
        projectionMatrix = glm::ortho(left, right, bottom , top);
        
        
    }
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
        -0.5f, -0.5f, 0.0f, // Bottom left
         0.5f, -0.5f, 0.0f, // Bottom right
         0.0f,  0.5f, 0.0f,  // Top
    };
    vbo.setData(vertices);

    GLVertexArray vao;
    vao.addVertexBuffer(vbo, 0, 3, GL_FLOAT); // Add vertex buffer to VAO with index 0


  

    while(!glfwWindowShouldClose(this->window)) { 
        // glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth
        processInput();


        drawShader->use();
        vao.drawArrays(GL_TRIANGLES, 0, 4); // Draw the triangle using the VAO
        graph.render(); // Render the grid

        glBindVertexArray(0);
        glfwPollEvents();
        glfwSwapBuffers(window); // Swap the front and back buffers
    } 
}




void Application::mouse_callback(GLFWwindow* window,double xPos, double yPos)
{ 

}

void Application::frameBufferSize_callback(GLFWwindow* window,int  width, int height) { 
    glViewport(0,  0,width, height) ;
    Application *app = getApplicationPtr(window);
    if(app ) { 
        app->WIN_WIDTH = width;
        app->WIN_HEIGHT = height;
    }

}

void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) { 
    std::cout << yoffset<< std::endl;
}


void Application::setUpCallbacks() { 
    glfwSetFramebufferSizeCallback(this->window, frameBufferSize_callback);
    glfwSetCursorPosCallback(this->window,mouse_callback);
    glfwSetScrollCallback(this->window, scroll_callback);
}





