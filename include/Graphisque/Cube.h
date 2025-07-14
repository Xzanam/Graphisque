#ifndef CUBE_H
#define CUBE_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "IRenderable.h"
#include "Shader.h"
#include <memory>

class Cube:public IRenderable{ 
    public: 
        Cube();
        void render(const Camera& camera) override;
        void update(float deltaTime) override;
        void setPosition(const glm::vec3& pos);
        void setModelMatrix(const glm::mat4& model);
        void setShader(std::shared_ptr<Shader> shader);



    private:
        glm::mat4 modelMatrix;
        glm::vec3 position;
        std::shared_ptr<VertexBuffer> _vbo;
        std::shared_ptr<VerteXArray> _vao;
        std::shared_ptr<Shader> _shader;
        void setupCube();

        std::vector<glm::vec3> vertices;


};

#endif //CUBE_H