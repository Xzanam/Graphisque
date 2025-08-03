#ifndef GLOBALS_H
#define GLOBALS_H
#include <memory>
#include "Shader.h"

extern const int G_WinWidth;  // Default window width

extern const int G_WinHeight; // Default window height

using SharedPtr = std::shared_ptr<Shader>;


namespace GLOBAL
{
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.01f;
    const float ZOOM = 45.0f;
    extern bool IS_CURSOR_DISABLED;

}





#endif // GLOBALS_H