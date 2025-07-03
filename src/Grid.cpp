#include "Grid.h"

// Constructor
Grid::Grid(float cellSize, int gridLinesX, int gridLinesY, float lineWidth)
    : m_cellSize(cellSize), m_lineWidth(lineWidth), m_gridLinesX(gridLinesX), m_gridLinesY(gridLinesY),
      m_zoomLevel(1.0f), m_panOffset(0.0f), m_minZoom(0.1f), m_maxZoom(10.0f), m_viewportWidth(800.0f),
      m_viewportHeight(600.0f), m_majorGridInterval(10), m_adaptiveGrid(true), m_adaptiveGridThreshold(0.5f),
      m_vertexCount(0) {

    // Set default colors
    setMajorGridColor(0.8f, 0.8f, 0.8f, 1.0f);
    setMinorGridColor(0.4f, 0.4f, 0.4f, 0.8f);

    m_lastUpdateTime = std::chrono::steady_clock::now();
}

void Grid::initialize() {
    // Initialize OpenGL buffers
    try {
        m_vertexBuffer = std::make_unique<GLBuffer>(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
        m_vertexArray = std::make_unique<GLVertexArray>();
        generateGridGeometry();
    } catch (const std::exception &e) {
        throw std::runtime_error("Failed to initialize grid buffers: " + std::string(e.what()));
    }
}


void Grid::setShader(const Shader &shader) {
    m_gridShader = shader;
}


void Grid::generateGridGeometry() { 
  float padding = 50.0f;

  float halfWidth = (m_gridLinesX / 2 + padding) * m_cellSize;;
  float halfHeight = (m_gridLinesY / 2 + padding) * m_cellSize; 

  for(int i = -(m_gridLinesX / 2 + padding) ;  i<= (m_gridLinesX / 2 + padding) ; ++i ){ 
    float x = i  * m_cellSize;
    m_vertices.push_back(x);
    m_vertices.push_back(-halfHeight);
    m_vertices.push_back(0.0f); // z coordinate


    m_vertices.push_back(x);
    m_vertices.push_back(halfHeight);
    m_vertices.push_back(0.0f); // z coordinate
  }


  for(int i = -(m_gridLinesY / 2 + padding) ;  i<= (m_gridLinesY / 2 + padding) ; ++i ){ 
    float y = i * m_cellSize;
    m_vertices.push_back(-halfWidth);
    m_vertices.push_back(y);
    m_vertices.push_back(0.0f); // z coordinate

    m_vertices.push_back(halfWidth);
    m_vertices.push_back(y);
    m_vertices.push_back(0.0f); // z coordinate
  } 

  m_vertexCount = m_vertices.size() / 3;
  updateBuffers();
}


void Grid::updateBuffers(){ 
  if(m_vertices.empty() || !m_vertexBuffer || !m_vertexArray) {
    return;
  }


  try{ 
    m_vertexBuffer->setData(m_vertices);
    m_vertexArray->addVertexBuffer(*m_vertexBuffer, 0, 3, GL_FLOAT);
  } catch (const std::exception &e) {
    throw std::runtime_error("Failed to update grid buffers: " + std::string(e.what()));
  }

}


void Grid::update(float deltaTime) { 
  if(!m_smoothTransitions) { 
    m_panOffset = m_targetPanOffset;
    m_zoomLevel = m_targetZoomLevel;
    return;
  }

  glm::vec2 panDelta = m_targetPanOffset - m_panOffset;
  if(glm::length(panDelta) > m_panSmoothness) {
    m_panOffset += panDelta * deltaTime * m_panSmoothness;
  } else { 
    m_panOffset = m_targetPanOffset;
  }


  float zoomDelta = m_targetZoomLevel - m_zoomLevel; 
  if(std::abs(zoomDelta) > 0.001f)  {
    m_zoomLevel += zoomDelta * deltaTime * m_zoomSmoothness;
  } else { 
    m_zoomLevel = m_targetZoomLevel;
  }

  //Clamping the zoom value
  m_zoomLevel = std::clamp(m_zoomLevel, m_minZoom, m_maxZoom);
  m_targetZoomLevel = std::clamp(m_targetZoomLevel, m_minZoom, m_maxZoom);
}


void Grid::render()  const{ 
  if(!m_vertexArray || m_vertexCount == 0 || m_gridShader.ID ==0){
    return;
  }
  glLineWidth(m_lineWidth);

  m_gridShader.use();

  glm::mat4 view = getViewMatrix();
  glm::mat4 projection = getProjectionMatrix();
  glm::mat4 mvp = projection * view;

  m_gridShader.setMat4("mvp", mvp);
  m_gridShader.setVec4("majorGridColor", m_majorGridColor);
  m_gridShader.setVec4("minorGridColor", m_minorGridColor);
  m_gridShader.setInt("majorGridInterval", m_majorGridInterval);
  m_gridShader.setFloat("cellSize", m_cellSize);
  m_gridShader.setFloat("zoomLevel", m_zoomLevel);  

  try{ 
    m_vertexArray->drawArrays(GL_LINES, 0,static_cast<GLsizei>(m_vertexCount));
  }catch (const std::exception &e) { 
    std::cerr << "Error rendering grid: " << e.what() << std::endl;
  }
  glLineWidth(1.0f); // Reset line width
}

void Grid::handleMouseButton(int button, int action,int mods, double xPos, double yPos) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            m_mouseState.leftButtonDown = true;
            m_mouseState.dragStart = glm::vec2(xPos, yPos);
            m_mouseState.lastPosition = glm::vec2(xPos, yPos);
            m_mouseState.isDragging = false; // Reset dragging state on press
        } else if (action == GLFW_RELEASE) {
            m_mouseState.leftButtonDown = false;
            m_mouseState.isDragging = false;
        }
    }
}


void Grid::handleMouseMove(double xPos, double yPos){ 
  glm::vec2 currentPos(xPos, yPos);

  if(m_mouseState.leftButtonDown){ 
    glm::vec2 dragDelta = currentPos - m_mouseState.dragStart;
    if(!m_mouseState.isDragging && glm::length(dragDelta) > m_mouseState.dragThreshold) {
        m_mouseState.isDragging = true;
    }

    if(m_mouseState.isDragging) { 
      glm::vec2 mouseDelta = currentPos - m_mouseState.lastPosition;
      
      //convert mouse delt ato world coordinates;
      float scale = 1.0f /  m_zoomLevel;
      glm::vec2 worldDelta = mouseDelta * scale;

      addPan(-worldDelta.x, worldDelta.y,true); // Invert Y for OpenGL coordinates
    }
  }

  m_mouseState.lastPosition = currentPos;
  m_mouseState.position = currentPos;
}

void Grid::handleMouseScroll(double xOffset, double yOffset) {
  float zoomFactor = 1.0f + yOffset * 0.1f; // Adjust zoom sensitivity
  std::cout <<"Mouse state position: " << m_mouseState.position.x << ", " << m_mouseState.position.y << std::endl;
  glm::vec2 mouseWorld = screenToWorld(m_mouseState.position);
  addZoom(zoomFactor-1.0f, mouseWorld,false); // Zoom towards mouse position
}


void Grid::setPan(float x, float y , bool smooth) { 
  if(smooth && m_smoothTransitions) {
    m_targetPanOffset = glm::vec2(x, y);
  } else {
    m_panOffset = m_targetPanOffset = glm::vec2(x, y);
  }
}



void Grid::addPan(float deltaX, float deltaY, bool smooth) {
    if (smooth && m_smoothTransitions) {
        m_targetPanOffset += glm::vec2(deltaX, deltaY);
    } else {
        m_panOffset += glm::vec2(deltaX, deltaY);
        m_targetPanOffset = m_panOffset; // Ensure target matches current pan
    }
}

void Grid::setZoom(float zoom, bool smooth) {
    zoom = std::clamp(zoom, m_minZoom, m_maxZoom); // Clamp zoom to limits
    if (smooth && m_smoothTransitions) {
        m_targetZoomLevel = zoom ;
    } else {
        m_zoomLevel = m_targetZoomLevel = zoom;
    }
}

void Grid::setZoom(float zoom, const glm::vec2& center, bool smooth) {
    zoom = std::clamp(zoom, m_minZoom, m_maxZoom); // Clamp zoom to limits

    glm::vec2 worldCenter = center;
    glm::vec2 beforeZoom = worldCenter - m_panOffset;

    float oldZoom = smooth? m_targetZoomLevel : m_zoomLevel;
    float scaleFactor = zoom / oldZoom; 


    glm::vec2 afterZoom = beforeZoom / scaleFactor;
    glm::vec2 panDelta = afterZoom - beforeZoom;  


    if (smooth && m_smoothTransitions) {
        m_targetZoomLevel = zoom;
        m_targetPanOffset += panDelta; // Adjust pan offset to keep center in place
    } else {
        m_zoomLevel = m_targetZoomLevel = zoom;
        m_panOffset += panDelta; // Adjust pan offset to keep center in place
        m_targetPanOffset = m_panOffset; // Ensure target matches current pan
    }
}


void Grid::addZoom(float deltaZoom, bool smooth) {
    float newZoom =(smooth ? m_targetZoomLevel: m_zoomLevel) + deltaZoom;
    setZoom(newZoom, smooth);
}


void Grid::addZoom(float deltaZoom, const glm::vec2& center, bool smooth) {
  float currentZoom = smooth ? m_targetZoomLevel : m_zoomLevel;
  float newZoom = currentZoom * (1.0f + deltaZoom);
  setZoom(newZoom, center, smooth);
} 

void Grid::resetView(bool smooth) {
  setPan(0.0f, 0.0f, smooth);
  setZoom(1.0f, smooth);
} 

void Grid::setViewport(float width, float height) {
    m_viewportWidth = width;
    m_viewportHeight = height;
}

//Helper Methods
glm::vec2 Grid::screenToWorld(const glm::vec2& screenPos) const {
    // Convert screen coordinates to world coordinates
    glm::vec2 ndc = glm::vec2(
        (2.0f * screenPos.x) / m_viewportWidth - 1.0f,
        1.0f - (2.0f * screenPos.y) / m_viewportHeight
    );
    
    // Apply inverse transformations
    glm::vec2 world = ndc;
    world *= glm::vec2(m_viewportWidth, m_viewportHeight) * 0.5f / m_zoomLevel;
    world += m_panOffset;
    
    return world;
}

glm::mat4 Grid::getViewMatrix() const {
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::scale(view, glm::vec3(m_zoomLevel, m_zoomLevel, 1.0f));
    view = glm::translate(view, glm::vec3(-m_panOffset, 0.0f));
    return view;
}

glm::mat4 Grid::getProjectionMatrix() const {
    float aspect = m_viewportWidth / m_viewportHeight;
    float halfHeight = m_viewportHeight * 0.5f;
    float halfWidth = halfHeight * aspect;
    
    return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);
}






// Color setters
void Grid::setMajorGridColor(float r, float g, float b, float a) {
    m_majorGridColor = glm::vec4(
        std::clamp(r, 0.0f, 1.0f),
        std::clamp(g, 0.0f, 1.0f),
        std::clamp(b, 0.0f, 1.0f),
        std::clamp(a, 0.0f, 1.0f)
    );
}

void Grid::setMinorGridColor(float r, float g, float b, float a) {
    m_minorGridColor = glm::vec4(
        std::clamp(r, 0.0f, 1.0f),
        std::clamp(g, 0.0f, 1.0f),
        std::clamp(b, 0.0f, 1.0f),
        std::clamp(a, 0.0f, 1.0f)
    );
}

void Grid::setMajorGridColor(const glm::vec4& color) {
    m_majorGridColor = glm::clamp(color, 0.0f, 1.0f);
}

void Grid::setMinorGridColor(const glm::vec4& color) {
    m_minorGridColor = glm::clamp(color, 0.0f, 1.0f);
}


// Animation settings
void Grid::setSmoothTransitions(bool enabled) {
    m_smoothTransitions = enabled;
}

void Grid::setPanSmoothness(float smoothness) {
    m_panSmoothness = std::max(0.1f, smoothness);
}

void Grid::setZoomSmoothness(float smoothness) {
    m_zoomSmoothness = std::max(0.1f, smoothness);
}

void Grid::setZoomLimits(float minZoom, float maxZoom) {
    m_minZoom = std::max(0.01f, minZoom);
    m_maxZoom = std::max(m_minZoom, maxZoom);
}

// Utility methods
glm::vec2 Grid::getWorldPosition(double screenX, double screenY) const {
    return screenToWorld(glm::vec2(screenX, screenY));
}

glm::vec2 Grid::getScreenPosition(const glm::vec2& worldPos) const {
    return worldToScreen(worldPos);
}

glm::vec2 Grid::worldToScreen(const glm::vec2& worldPos) const {
    glm::vec2 relative = (worldPos - m_panOffset) * m_zoomLevel;
    return glm::vec2(
        (relative.x + m_viewportWidth * 0.5f),
        (m_viewportHeight * 0.5f - relative.y)
    );
}

// Additional setters
void Grid::setCellSize(float cellSize) {
    m_cellSize = std::max(0.01f, cellSize);
    generateGridGeometry();
}

void Grid::setGridSize(int linesX, int linesY) {
    m_gridLinesX = std::max(2, linesX);
    m_gridLinesY = std::max(2, linesY);
    generateGridGeometry();
}

void Grid::setLineWidth(float width) {
    m_lineWidth = std::max(0.1f, width);
}

void Grid::setMajorGridInterval(int interval) {
    m_majorGridInterval = std::max(1, interval);
}

void Grid::setAdaptiveGrid(bool enabled) {
    m_adaptiveGrid = enabled;
}

void Grid::setAdaptiveGridThreshold(float threshold) {
    m_adaptiveGridThreshold = std::max(0.1f, threshold);
}

void Grid::fitToView(bool smooth) {
    // Calculate zoom to fit grid in view
    float gridWidth = m_gridLinesX * m_cellSize;
    float gridHeight = m_gridLinesY * m_cellSize;
    
    float zoomX = m_viewportWidth / gridWidth;
    float zoomY = m_viewportHeight / gridHeight;
    float zoom = std::min(zoomX, zoomY) * 0.8f; // 80% of max zoom for padding
    
    setPan(0.0f, 0.0f, smooth);
    setZoom(zoom, smooth);
}

