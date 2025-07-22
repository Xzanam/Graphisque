#ifndef AXES_H
#define AXES_H

#include "Buffer.h"
#include <memory>
#include <cmath>


class Axes { 
    public: 
        Axes(){ 

        }

}; 


class Cylinder{ 
    private: 
        std::unique_ptr<VerteXArray> _vao;
        std::unique_ptr<VertexBuffer> _vbo;
        std::unique_ptr<ElemBuffer> _ebo;

        std::vector<glm::vec3> _vertices;

        std::vector<int> _indices;

        float _r, _h;
        int _seg;
        void genVertices(){
            float x, z;
            float angleStep = 1 / (float) _seg;
            for(int i = 0 ; i < _seg; ++i) { 
                float angle = i * angleStep;
                x = _r * cos(2 * M_PI * angle);
                z = _r * sin(2 * M_PI *angle);
                _vertices.emplace_back(x, _h / 2, z);
                _vertices.emplace_back(x, -_h / 2, z);
            }

            _vertices.emplace_back(0.0f, _h / 2, 0.0f);
            _vertices.emplace_back(0.0f, -_h / 2, 0.0f);
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
        Cylinder(float radius,float height, int segments)
            : _r(radius), _h(height), _seg(segments) { 
            _vao = std::make_unique<VerteXArray>();
            _vbo = std::make_unique<VertexBuffer>();
            _ebo = std::make_unique<ElemBuffer>(GL_ELEMENT_ARRAY_BUFFER);
            initBuffers();
        } 

        void initBuffers() { 
            genVertices();
            genIndices();
            _vbo->setData(_vertices);  // Error prone
            _ebo->setData(_indices);
            _vao->addVertexBuffer(*_vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            _vao->setElementBuffer(*_ebo);
        }

        void draw(){ 
            _vao->drawElements(GL_TRIANGLES,_indices.size(), GL_UNSIGNED_INT, 0);
        }




};



#endif