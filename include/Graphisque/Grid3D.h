#include <iostream> 
 


#include "Shader.h"

#include "IRenderable.h"

struct GridConfig { 
    float resolution = 30.0f;
    float bounds = 30.0f;
    int frameScaling = resolution / 100 + 1;
    int frameSpacing = 10;
    bool drawGrid = true;
    bool drawNegatives = false;
};

class Grid3D : public IRenderable{ 
    private: 
        glm::mat4 _modelMatrix; // Model matrix for the grid
        GridConfig _config;
        std::shared_ptr<Shader> _shader;
        size_t _vertexCount;

        void initGrid();
    public:
        Grid3D(GridConfig& configuration,std::shared_ptr<Shader> shader); 
        void update(float deltaTime) override;
        void render(const Camera& camera) override;

};



