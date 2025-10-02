//
// Created by Janam Shrestha on 10/2/25.
//

#ifndef GLUTILS_H
#define GLUTILS_H
#include "glad/gl.h"

namespace Renderer::Utils {

    const char* GLDebugSourceToString(GLenum source);
    const char* GLDebugTypeToString(GLenum type);
    const char* GLDebugSeverityToString(GLenum severity);

    void InitOpenGLDebugMessageCallback();
}

#endif //GLUTILS_H
