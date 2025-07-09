#ifndef IRENDERABLE_H
#define IRENDERABLE_H
#include "Camera.h"
#include "Buffer.h"

class IRenderable { 
    protected:
        std::shared_ptr<GLBuffer> _vbo;
        std::shared_ptr<VerteXArray> _vao;

    public : 
        virtual void update(float deltaTime) = 0;
        virtual void render(const Camera& camera) = 0;
        virtual ~IRenderable() = default;
};

#endif // IRENDERABLE_H 