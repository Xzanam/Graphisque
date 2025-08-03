//
// Created by Janam on 7/30/25.
//
#ifndef EQUATIONS_H
#define EQUATIONS_H

#include <cmath>
#include<vector>
#include "Shader.h"


inline double f (float x, float y) {
    return (sin(x) * sin(x) + cos(y) * cos(y));
}

inline float sphereEqn(float x, float y) {
    return sqrt(25 - x*x - y*y);
}

const float lim = 5.0f;
class Equation {
    std::vector<glm::vec3> graphPoints;
    float xMin = -lim;
    float xMax = lim;
    float yMin = -lim;
    float yMax = lim;
    float step  = 0.25f;
    std::unique_ptr<VertexBuffer> _vbo;
    std::unique_ptr<VerteXArray> _vao;
    glm::vec3 color = glm::vec3(0.4f, 0.1f, 0.6f);


public:
    Equation() {
        _vbo = std::make_unique<VertexBuffer>();
        _vao = std::make_unique<VerteXArray>();
        init();
    };
    void init() {
        for (float x = xMin ; x < xMax ; x += step) {
            for (float y = yMin ; y < yMax ; y += step) {
                float z = f(x, y);
                graphPoints.emplace_back(x, z, y);
            }
        }
        _vbo->setData(graphPoints);
        _vao->addVertexBuffer(*_vbo, 0, 3, GL_FLOAT);
    }
    void draw(const std::shared_ptr<Shader>& shader) {
        shader->setMat4("model", glm::mat4(1.0f));
        shader->setVec3("objectColor", color);
        glPointSize(3.0f);
        _vao->drawArrays(GL_POINTS, 0, graphPoints.size());
    }

};

#endif //EQUATIONS_H
