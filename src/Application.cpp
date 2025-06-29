#include "Application.h"
#include "GLBuffer.h"
#include "Shader.h"



Application::Application(const std::string& title , int width , int height) : title(title), WIN_WIDTH(width), WIN_HEIGHT(height), window(nullptr), lastX(width/2.0f), lastY(height/2.0f) { 
    std::cout << "Application started with title: " << title 
              << ", width: " << width 
              << ", height: " << height << std::endl; 
            init();
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
 
void Application::processInput() { 
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { 
        glfwSetWindowShouldClose(this->window, true); 
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

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    vbo.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // Unbind VAO
    vbo.unbind();

    Shader myShader = Shader("./shaders/vertex.vert", "./shaders/fragment.frag");



    while(!glfwWindowShouldClose(this->window)) { 
        // glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth

        myShader.use();
        glBindVertexArray(vao); // Bind the VAO
        glDrawArrays(GL_LINE_STRIP, 0, 4); // Draw the triangle
        glBindVertexArray(0);
        


        glfwPollEvents();
        glfwSwapBuffers(window); // Swap the front and back buffers
    } 
     glDeleteVertexArrays(1, &vao); // Delete the VAO
}



