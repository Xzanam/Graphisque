#include "Grid3D.h"


Grid3D::Grid3D(GridConfig& configuration, Shader& shader) : _config(configuration) , _shader(shader) {
    _vbo= std::make_shared<VertexBuffer>();
    _vao= std::make_shared<VerteXArray>();
    initGrid();
}


void Grid3D::initGrid(){ 
    std::vector<glm::vec3> gridVertices;

    int halfSpan = _config.frameSpacing / 2;
    for(int i = 0; i <= _config.frameSpacing; ++i) { 
        float offset = (halfSpan - i) * _config.frameScaling; 

        gridVertices.emplace_back(offset, 0.0f, -halfSpan * _config.frameScaling);
        gridVertices.emplace_back(offset, 0.0f, halfSpan * _config.frameSpacing);

        gridVertices.emplace_back(-halfSpan *  _config.frameSpacing, 0.0f, offset);
        gridVertices.emplace_back(halfSpan *  _config.frameSpacing, 0.0f, offset);
    }

    _vbo->setData(gridVertices.data(), gridVertices.size());
    _vao->addVertexBuffer(*_vbo,0, 3,GL_FLOAT);
    _vertexCount =(GLsizei)gridVertices.size();
}


void Grid3D::render(const Camera& camera) { 
    _shader.use();
    _shader.setMat4("view",camera.getViewMatrix());
    _shader.setMat4("projection", camera.getProjectionMatrix());
    _vao->drawArrays(GL_LINES, 0, _vertexCount);
}


void Grid3D::update(float deltaTime) { 
    std::cout << "Not implemented"<<std::endl;
}