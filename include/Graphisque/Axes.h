#ifndef AXES_H
#define AXES_H

#include "Buffer.h"
#include <memory>
#include <cmath>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Camera.h"



class Shape { 
    protected:  
        std::unique_ptr<VerteXArray> _vao;
        std::unique_ptr<VertexBuffer> _vbo;
        std::unique_ptr<ElemBuffer> _ebo;
        std::vector<glm::vec3> _vertices;
        std::vector<int> _indices;

    public: 
        Shape() { 
            _vao = std::make_unique<VerteXArray>();
            _vbo = std::make_unique<VertexBuffer>();
            _ebo = std::make_unique<ElemBuffer>(GL_ELEMENT_ARRAY_BUFFER);
        } 
        virtual ~Shape() = default; 
        virtual void draw()  = 0 ;

};


class Cone : public Shape{ 
    private: 
        float _r, _h;
        int _seg;
        glm::vec3 _center;
        void genVertices() { 
            float x, y, z;
            float angleStep = 1 / (float) _seg;
            for(int i = 0 ; i < _seg; ++i) { 
                float angle = i * angleStep;
                x = _r * cos(2 * M_PI * angle);
                z = _r * sin(2 * M_PI * angle);
                _vertices.emplace_back(x, 0, z);
            }
            _vertices.emplace_back(_center); // the center of the base is at index _seg;
            _vertices.emplace_back(0, _h, 0); // the top vertex at index _seg + 1
        }
        void genIndices(){ 
            for(int i = 0; i < _seg  ; ++i) { 
                //lateral surface
                _indices.push_back(_seg + 1);
                _indices.push_back(i);
                _indices.push_back((i+1) % _seg);


                _indices.push_back(_seg); 
                _indices.push_back(i);
                _indices.push_back((i + 1) % _seg);

            }


        }
    public:
        Cone(float radius, float height, int segments = 360) : Shape(),  _seg(segments), _r(radius), _h(height), _center(glm::vec3(0.0f, 0.0f, 0.0f)){
            initBuffers();
        }

        void initBuffers() { 
            genVertices();
            genIndices();
            _vbo->setData(_vertices);  // Error prone
            _ebo->setData(_indices);
            _vao->addVertexBuffer(*_vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            _vao->setElementBuffer(*_ebo);
            _vao->unbind();
        }

        void draw() override{   
            _vao->drawElements(GL_TRIANGLES,_indices.size(), GL_UNSIGNED_INT, 0);
        }

};



class Cylinder : public Shape{ 
    private: 
        float _r, _h;
        int _seg;
        void genVertices(){
            float x, z;
            float angleStep = 1 / (float) _seg;
            for(int i = 0 ; i < _seg; ++i) { 
                float angle = i * angleStep;
                x = _r * cos(2 * M_PI * angle);
                z = _r * sin(2 * M_PI *angle);
                _vertices.emplace_back(x, _h , z);
                _vertices.emplace_back(x, 0,  z);
            }

            _vertices.emplace_back(0.0f, _h , 0.0f); // center of the top face
            _vertices.emplace_back(0.0f, 0, 0.0f); //center of the bottom face
        } 
        void genIndices(){ 
            int topCenterIdx = 2 * _seg;
            int bottomCenterIdx = 2 * _seg + 1;

            //Top face
            for(int i = 0; i < _seg; ++i) { 
                _indices.push_back(topCenterIdx);
                _indices.push_back(((i * 2) % (2 * _seg)));
                _indices.push_back(((i+1) % _seg ) * 2);
            }

            //Bottom Face
            for(int i = 0; i < _seg ; ++i) { 
                _indices.push_back(bottomCenterIdx);
                _indices.push_back(((i + 1) % _seg ) * 2 + 1);
                _indices.push_back(((i * 2 + 1) % (2 * _seg)));

            }

            //side faces
            for(int i = 0; i < _seg; ++i) { 
                int top1 = (i * 2) % (2 * _seg);
                int bottom1 = (i * 2  + 1) % (2 * _seg); 
                int top2  = ((i + 1) % _seg) * 2;
                int bottom2 = ((i + 1) % _seg) * 2 + 1;

                _indices.push_back(top1);
                _indices.push_back(bottom1);
                _indices.push_back(top2);

                _indices.push_back(top2);
                _indices.push_back(bottom1);
                _indices.push_back(bottom2);

            }

        }



    public: 
        Cylinder():Shape() { }
        ~Cylinder() = default;
        Cylinder(float radius,float height, int segments = 360)
            :Shape(),  _r(radius), _h(height), _seg(segments) { 
            initBuffers();
        } 

        void initBuffers() { 
            genVertices();
            genIndices();
            _vbo->setData(_vertices);  // Error prone
            _ebo->setData(_indices);
            _vao->addVertexBuffer(*_vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            _vao->setElementBuffer(*_ebo);
            _vao->unbind();
        }

        void draw() override{ 
            _vao->drawElements(GL_TRIANGLES,_indices.size(), GL_UNSIGNED_INT, 0);
        }
        float getHeight() { 
            return this-> _h;
        }

};


class Axis{
    private: 
        Cylinder _shaft;
        Cone _arrow;
        glm::mat4 _model;
        bool _drawNegative;
        glm::vec3 _color;

    public: 
        Axis(float shaftLength = 20.0f, float shaftRadius = 0.2f, float coneHeight = 2.0f, float coneRadius = 0.8f, bool drawNeg = true) 
        : _shaft(shaftRadius, shaftLength), _arrow(coneRadius, coneHeight) ,_color(glm::vec3(0.8f, 0.8f, 0.8f)), _drawNegative(drawNeg)
        { 
            _model = glm::mat4(1.0f);
        }

        void setModel(const glm::mat4& m) { _model = m;}
        void setColor(glm::vec3& color) { 
            _color = color;
        }

        void draw(const std::shared_ptr<Shader>& shader) { 
            shader-> use();
            shader->setVec3("objectColor", _color);
            shader->setMat4("model", _model);
            _shaft.draw();

            glm::mat4 arrowModel = glm::translate(_model, glm::vec3(0.0f, _shaft.getHeight() , 0.0f));
            shader->setMat4("model", arrowModel);
            _arrow.draw();

            if(_drawNegative) { 
                glm::mat4 negModel = _model * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f));
                shader->setMat4("model", negModel);
                _shaft.draw();

                glm::mat4 arrowModelNeg = glm::translate(negModel, glm::vec3(0.0f, _shaft.getHeight(), 0.0f)) ;
                shader->setMat4("model", arrowModelNeg);
                _arrow.draw();
            }
        }

};



class Axes { 
    private: 
        Axis xAxis , yAxis, zAxis;
        glm::vec3 xAxisColor = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 yAxisColor = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 zAxisColor = glm::vec3(0.0f, 0.0f, 1.0f);


    public: 
        Axes(float shaftLength, float shaftRadius, float coneHeight, float coneRadius, bool showNeg = true) 
        : xAxis(shaftLength, shaftRadius, coneHeight, coneRadius, showNeg)
        , yAxis(shaftLength, shaftRadius, coneHeight, coneRadius, showNeg)
        , zAxis(shaftLength, shaftRadius, coneHeight, coneRadius, showNeg)
        {
            xAxis.setModel(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 0, -1)));
            yAxis.setModel(glm::mat4(1.0f));
            zAxis.setModel(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0))); //camera convention might have an effect here
            xAxis.setColor(xAxisColor);
            yAxis.setColor(yAxisColor);
            zAxis.setColor(zAxisColor);
        }

        void setAxisColor(char axis, glm::vec3& color) { 
            switch (axis) { 
                case 'x': 
                case 'X':
                    this->xAxisColor = color;
                    xAxis.setColor(xAxisColor);
                    break;
                case 'y': 
                case 'Y':
                    this->yAxisColor = color;
                    yAxis.setColor(yAxisColor);
                    break;
                case 'z': 
                case 'Z':
                    this->zAxisColor = color;
                    zAxis.setColor(zAxisColor);
                    break;
                default: 
                    std::cerr << "Error Setting color, specify a valid axis /x/y/z" <<std::endl;
                    break;
            }
                


        }

        void draw(const std::shared_ptr<Shader>& shader) { 
            xAxis.draw(shader);
            yAxis.draw(shader);
            zAxis.draw(shader);
        }


}; 





#endif