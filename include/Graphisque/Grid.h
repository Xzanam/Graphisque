#include "GLVertexArray.h"
#include <glad/glad.h>
#include <memory>
#include <algorithm>

class Grid {
  private:
    float m_cellSize;
    float m_lineWidth;
    float m_gridLinesX;
    int m_gridLinesY;

    float viewPortWidth, viewPortHeight;

    std::unique_ptr<GLBuffer> m_vertexBuffer; // Use unique_ptr for automatic memory management
    std::unique_ptr<GLVertexArray> m_vertexArray;

    std::vector<float> m_vertices; // Store grid vertices
    size_t m_vertexCount;

    float m_majorGridColor[4];
    float m_minorGridColor[4];
    float m_majorGridInterval;

     // View parameters
    float m_panX, m_panY;       // Pan offset
    float m_zoomLevel;          // Zoom level (1.0 = normal)
    float m_minZoom, m_maxZoom; // Zoom limits

    // Viewport parameters
    float m_viewportWidth, m_viewportHeight;

    bool m_adaptiveGrid;
    float m_adaptiveGridThreshold;


    Shader m_gridShader;

  public:
    // Constructor
    Grid(float cellSize = 0.2f, int gridLinesX = 10, int gridLinesY = 10, 
         float lineWidth = 1.0f)
        : m_cellSize(cellSize), m_lineWidth(lineWidth), 
          m_gridLinesX(gridLinesX), m_gridLinesY(gridLinesY),
          m_panX(0.0f), m_panY(0.0f), m_zoomLevel(1.0f),
          m_minZoom(0.1f), m_maxZoom(10.0f),
          m_viewportWidth(800.0f), m_viewportHeight(600.0f),
          m_majorGridInterval(10), m_adaptiveGrid(true), m_adaptiveGridThreshold(0.5f),
          m_vertexCount(0) {
        
        // Set default colors
        setMajorGridColor(0.8f, 0.8f, 0.8f, 1.0f);
        setMinorGridColor(0.4f, 0.4f, 0.4f, 0.8f);
        
        // Initialize OpenGL objects
        initializeBuffers();
        generateGrid();
    }

        // Destructor
    ~Grid() = default;

     // Delete copy constructor and assignment
    Grid(const Grid&) = delete;
    Grid& operator=(const Grid&) = delete;
    
    // Move constructor and assignment
    Grid(Grid&&) = default;
    Grid& operator=(Grid&&) = default;

    void setShader(const Shader& shader) {
        m_gridShader = shader;
    }
    
    // Initialize OpenGL buffers
    void initializeBuffers() {
        try {
            m_vertexBuffer = std::make_unique<GLBuffer>(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
            m_vertexArray = std::make_unique<GLVertexArray>();
        } catch (const std::exception& e) {
            throw std::runtime_error("Failed to initialize grid buffers: " + std::string(e.what()));
        }
    }

     // Generate grid geometry
    void generateGrid() {
        m_vertices.clear();
        
        // float halfWidth = (m_gridLinesX - 1) * m_cellSize * 0.5f;
        // float halfHeight = (m_gridLinesY - 1) * m_cellSize * 0.5f;
        float halfWidth = (m_gridLinesX /2 )  * m_cellSize;
        float halfHeight = (m_gridLinesY/2 ) * m_cellSize;

        
        // Generate vertical lines
        for (int i = -m_gridLinesX / 2; i < m_gridLinesX/2; ++i) {
            float x = i * m_cellSize;
            
            // Line start point
            m_vertices.push_back(x);
            m_vertices.push_back(-halfHeight);
            m_vertices.push_back(0.0f); // z coordinate
            
            // Line end point
            m_vertices.push_back(x);
            m_vertices.push_back(halfHeight);
            m_vertices.push_back(0.0f); // z coordinate
        }
        
        // Generate horizontal lines
        for (int i = -m_gridLinesY / 2 ; i < m_gridLinesY / 2; ++i) {
            float y = i * m_cellSize;
            
            // Line start point
            m_vertices.push_back(-halfWidth);
            m_vertices.push_back(y);
            m_vertices.push_back(0.0f); // z coordinate
            
            // Line end point
            m_vertices.push_back(halfWidth);
            m_vertices.push_back(y);
            m_vertices.push_back(0.0f); // z coordinate
        }
        
        m_vertexCount = m_vertices.size() / 3;
        
        // Upload to GPU
        updateBuffers();
    }

    // Update GPU buffers
    void updateBuffers() {
        if (m_vertices.empty() || !m_vertexBuffer || !m_vertexArray) {
            return;
        }
        
        try {
            // Upload vertex data
            m_vertexBuffer->setData(m_vertices);
            
            // Setup VAO
            m_vertexArray->addVertexBuffer(*m_vertexBuffer, 0, 3, GL_FLOAT);
            
        } catch (const std::exception& e) {
            throw std::runtime_error("Failed to update grid buffers: " + std::string(e.what()));
        }
    }

        // Render the grid
    void render() const {
        if (!m_vertexArray|| m_vertexCount == 0) {
            return;
        }
        
        // Set line width
        glLineWidth(m_lineWidth);
     

        if(m_gridShader.ID ==0) { 
            throw std::runtime_error("Grid shader is not initialized");
        }

        m_gridShader.use();
            // Pass uniform values
        m_gridShader.setVec4("majorGridColor", glm::vec4(m_majorGridColor[0], m_majorGridColor[1], m_majorGridColor[2], m_majorGridColor[3]));
        m_gridShader.setVec4("minorGridColor",glm::vec4(m_minorGridColor[0], m_minorGridColor[1], m_minorGridColor[2], m_minorGridColor[3]));
        m_gridShader.setInt("majorGridInterval", m_majorGridInterval);



       // Bind VAO and draw major grid lines
        m_gridShader.setBool("isMajorGrid", true);
        
        try {
            m_vertexArray->drawArrays(GL_LINES, 0, static_cast<GLsizei>(m_vertexCount));
        } catch (const std::exception& e) {
            // Log error but don't throw during rendering
            // In a real application, you might want to log this
        }
        
  
        glLineWidth(1.0f); // Reset line width
    }   // Color settings
    void setMajorGridColor(float r, float g, float b, float a = 1.0f) {
        m_majorGridColor[0] = std::clamp(r, 0.0f, 1.0f);
        m_majorGridColor[1] = std::clamp(g, 0.0f, 1.0f);
        m_majorGridColor[2] = std::clamp(b, 0.0f, 1.0f);
        m_majorGridColor[3] = std::clamp(a, 0.0f, 1.0f);
    }
    
    void setMinorGridColor(float r, float g, float b, float a = 1.0f) {
        m_minorGridColor[0] = std::clamp(r, 0.0f, 1.0f);
        m_minorGridColor[1] = std::clamp(g, 0.0f, 1.0f);
        m_minorGridColor[2] = std::clamp(b, 0.0f, 1.0f);
        m_minorGridColor[3] = std::clamp(a, 0.0f, 1.0f);
    }

     // Getters
    float getPanX() const { return m_panX; }
    float getPanY() const { return m_panY; }
    float getZoom() const { return m_zoomLevel; }
    float getCellSize() const { return m_cellSize; }
    float getLineWidth() const { return m_lineWidth; }
    
    const float* getMajorGridColor() const { return m_majorGridColor; }
    const float* getMinorGridColor() const { return m_minorGridColor; }
    
    bool isAdaptiveGrid() const { return m_adaptiveGrid; }



};
