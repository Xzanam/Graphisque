//
// Created by Janam Shrestha on 10/2/25.
//

#ifndef RENDERER_H
#define RENDERER_H


#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <filesystem>

namespace Renderer {
    struct Texture {
        GLuint Handle = 0;
        uint32_t Width = 0;
        uint32_t Height = 0;
    };
    struct Framebuffer {
        GLuint Handle = 0 ;
        Texture ColorAttachment;
    };
    Texture CreateTexture(int width, int height);
    Texture LoadTexture(const std::filesystem::path& path);
    Framebuffer CreateFramebufferWithTexture(const Texture texture);
    bool AttachTextureToFramebuffer(Framebuffer& framebuffer, const Texture& texture);
    void BlitFramebufferToSwapChain(const Framebuffer& framebuffer);


}


#endif //RENDERER_H
