#include "Grid3D.h"


Grid3D::Grid3D(GridConfig& configuration,std::shared_ptr<Shader> shader) : _config(configuration) , _shader(shader) {
    _vbo= std::make_shared<VertexBuffer>();
    _vao= std::make_shared<VerteXArray>();
    initGrid();
}


void Grid3D::initGrid() {
    std::vector<glm::vec3> gridVertices;

    int halfSpan = _config.frameSpacing / 2;
    float step = _config.frameScaling;

    float start = -halfSpan * step;
    float end =  halfSpan * step;

    // X-Y plane grid lines (at multiple Z levels)
    for (int z = -halfSpan; z <= halfSpan; ++z) {
        float zOffset = z * step;

        for (int i = -halfSpan; i <= halfSpan; ++i) {
            float offset = i * step;

            // Horizontal line (X direction)
            gridVertices.emplace_back(start, offset, zOffset);
            gridVertices.emplace_back(end,   offset, zOffset);

            // Vertical line (Y direction)
            gridVertices.emplace_back(offset, start, zOffset);
            gridVertices.emplace_back(offset, end,   zOffset);
        }
    }

    // Y-Z plane grid lines (at multiple X levels)
    for (int x = -halfSpan; x <= halfSpan; ++x) {
        float xOffset = x * step;

        for (int i = -halfSpan; i <= halfSpan; ++i) {
            float offset = i * step;

            // Horizontal line (Z direction)
            gridVertices.emplace_back(xOffset, offset, start);
            gridVertices.emplace_back(xOffset, offset, end);

            // Vertical line (Y direction)
            gridVertices.emplace_back(xOffset, start, offset);
            gridVertices.emplace_back(xOffset, end,   offset);
        }
    }

    // X-Z plane grid lines (at multiple Y levels)
    for (int y = -halfSpan; y <= halfSpan; ++y) {
        float yOffset = y * step;

        for (int i = -halfSpan; i <= halfSpan; ++i) {
            float offset = i * step;

            // Horizontal line (X direction)
            gridVertices.emplace_back(start, yOffset, offset);
            gridVertices.emplace_back(end,   yOffset, offset);

            // Vertical line (Z direction)
            gridVertices.emplace_back(offset, yOffset, start);
            gridVertices.emplace_back(offset, yOffset, end);
        }
    }

    _vbo->setData(gridVertices.data(), gridVertices.size() * sizeof(glm::vec3));
    _vao->addVertexBuffer(*_vbo, 0, 3, GL_FLOAT);
    _vertexCount = static_cast<GLsizei>(gridVertices.size());
}



void Grid3D::render(const Camera& camera) { 
    _shader->use();
    _shader->setMat4("model",glm::scale(glm::mat4(1.0f),glm::vec3(2, 2, 2)));
    _shader->setMat4("view",camera.getViewMatrix());
    // _shader->setMat4("projection", camera.getProjectionMatrix());
    _vao->drawArrays(GL_LINES, 0, _vertexCount);
}


void Grid3D::update(float deltaTime) { 
    std::cout << "Not implemented"<<std::endl;
}