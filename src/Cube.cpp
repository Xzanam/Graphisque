//CUBE.CPP
#include "Cube.h"


Cube::Cube(){ 
    _vbo = std::make_shared<VertexBuffer>();
    _vao = std::make_shared<VerteXArray>();
  
    modelMatrix = glm::mat4(1.0f);
    position = glm::vec3(0.0f, 0.0f, 0.0f);

      setupCube();

}

void Cube::setupCube(){ 
 vertices = {
    // Back face
    {-0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f,  0.5f, -0.5f},
    { 0.5f,  0.5f, -0.5f},
    {-0.5f,  0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},

    // Front face
    {-0.5f, -0.5f,  0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},
    {-0.5f, -0.5f,  0.5f},

    // Left face
    {-0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},

    // Right face
    { 0.5f,  0.5f,  0.5f},
    { 0.5f,  0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},

    // Bottom face
    {-0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f, -0.5f,  0.5f},
    {-0.5f, -0.5f,  0.5f},
    {-0.5f, -0.5f, -0.5f},

    // Top face
    {-0.5f,  0.5f, -0.5f},
    { 0.5f,  0.5f, -0.5f},
    { 0.5f,  0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f, -0.5f}
};





    _vbo->setData(vertices.data(),  sizeof(glm::vec3) * vertices.size()) ;
    _vao->addVertexBuffer(*_vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Cube::render(const Camera& camera){ 
    _shader->use(); 
    // _shader->setMat4("model", modelMatrix);
    _shader->setMat4("view", camera.getViewMatrix());
    _shader->setMat4("projection", camera.getProjectionMatrix());
    _vao->drawArrays(GL_TRIANGLES, 0, 36);

}

void Cube::setModelMatrix(const glm::mat4& model){ 
    modelMatrix = model;
}

void Cube::setPosition (const glm::vec3& pos) { 
    position = glm::vec3(pos);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Cube::update(float deltaTime){ 
    float speed = 1.0f;
    position += glm::vec3(0.0f, 0.0f, speed * deltaTime);
    setPosition(position);

}


void Cube::setShader(std::shared_ptr<Shader> shader) {
    _shader = shader;
}
