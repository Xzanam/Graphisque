#pragma once
#include <glad/glad.h>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <string>




class GLBuffer {
    private:
        GLuint m_bufferId;
        GLenum m_target; // Target can be GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, etc.
        GLenum m_usage; // Usage can be GL_STATIC_DRAW, GL_DYNAMIC_DRAW, etc.
        size_t m_size;
        bool m_initialized;
    public:
        GLBuffer(GLenum target = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_DRAW)
            :m_bufferId(0),  m_target(target), m_usage(usage), m_size(0), m_initialized(false) {
            glGenBuffers(1, &m_bufferId);
            if (m_bufferId == 0) {
                throw std::runtime_error("Failed to generate OpenGL buffer");
            }
        }

        ~GLBuffer() {
            if(m_bufferId != 0) {
                glDeleteBuffers(1, &m_bufferId);
            }
        }

        //the move constructor : noexcept is used to indicate that this constructor does not throw exceptions
        //this is important for performance reasons, as it allows the compiler to optimize the code better
        //when a class is moved, it means that the resources of the object are transferred to the new object
        //and the old object is left in a valid but unspecified state

        GLBuffer(GLBuffer&& other) noexcept
            : m_bufferId(other.m_bufferId), m_target(other.m_target), m_usage(other.m_usage),
              m_size(other.m_size), m_initialized(other.m_initialized) {
            other.m_bufferId = 0;
            other.m_size =  0;
            other.m_initialized  = false;
        }

        GLBuffer& operator=(GLBuffer&& other) noexcept {
            if (this != &other) {
                if (m_bufferId != 0) {
                    glDeleteBuffers(1, &m_bufferId);
                }
                m_bufferId = other.m_bufferId;
                m_target = other.m_target;
                m_usage = other.m_usage;
                m_size = other.m_size;
                m_initialized = other.m_initialized;

                other.m_bufferId = 0;
                other.m_size = 0;
                other.m_initialized = false;
            }
            return *this;
        }

        //Delete copy constructor and copy assignment operator
        GLBuffer(const GLBuffer&) = delete;
        GLBuffer& operator=(const GLBuffer&) = delete;


        void bind() const { 
            if( m_bufferId == 0) {
                throw std::runtime_error("Buffer not initialized");
            }
            glBindBuffer(m_target, m_bufferId); 
        }

        void unbind() const { 
            glBindBuffer(m_target, 0);  
        }

        template<typename T>
        void setData(const std::vector<T>& data) {
            if (data.empty()) {
                throw std::runtime_error("Data vector is empty");
            }
            setData(data.data(), data.size() * sizeof(T));
        }

        //set raw data in the buffer
        void setData(const void* data, size_t size){ 
            bind();
            glBufferData(m_target, size, data, m_usage);
            m_size = size;
            m_initialized = true;

            GLenum error = glGetError();
            if(error != GL_NO_ERROR) {
                std::cerr << "OpenGL Error: " << error << std::endl;
                throw std::runtime_error("Failed to set buffer data");
            }   
        }

        template<typename T>
        void updateData(const std::vector<T>& data, size_t offset = 0) {
            if (data.empty()) {
                throw std::runtime_error("Data vector is empty");
            }
            updateData(data.data(), data.size() * sizeof(T), offset);
        }


        void updateData(const void* data, size_t size, size_t offset = 0) {
            if(!m_initialized ) { 
                throw std::runtime_error("Buffer not initialized");
            }
            if (offset + size > m_size) {
                throw std::runtime_error("Update exceeds buffer size");
            }
            bind();
            glBufferSubData(m_target, offset, size, data);

            GLenum error = glGetError();
            if(error != GL_NO_ERROR) {
                std::cerr << "OpenGL Error: " << error << std::endl;
                throw std::runtime_error("Failed to update buffer data");
            }
        }
        
        //resizing the buffer is not a common operation in OpenGL
        //will lose the exisiting data in the buffer
        void resize(size_t newSize) { 
            bind();
            glBufferData(m_target, newSize, nullptr, m_usage);
            m_size = newSize;
            m_initialized= true;
        }

        // Map buffer for direct access (OpenGL 1.5+)
    void* map(GLenum access = GL_READ_WRITE) {
        if (!m_initialized) {
            throw std::runtime_error("Buffer must be initialized before mapping");
        }
        bind();
        void* ptr = glMapBuffer(m_target, access);
        if (!ptr) {
            throw std::runtime_error("Failed to map buffer");
        }
        return ptr;
    }

    // Unmap buffer
    bool unmap() {
        bind();
        return glUnmapBuffer(m_target) == GL_TRUE;
    }

    // Map buffer range (OpenGL 3.0+)
    void* mapRange(size_t offset, size_t length, GLbitfield access) {
        if (!m_initialized) {
            throw std::runtime_error("Buffer must be initialized before mapping");
        }
        if (offset + length > m_size) {
            throw std::runtime_error("Map range exceeds buffer size");
        }
        
        bind();
        void* ptr = glMapBufferRange(m_target, offset, length, access);
        if (!ptr) {
            throw std::runtime_error("Failed to map buffer range");
        }
        return ptr;
    }

    //Getters
    GLuint getID() const { return m_bufferId; }
    GLenum getTarget() const { return m_target; }
    GLenum getUsage() const { return m_usage; }
    size_t getSize() const { return m_size; }
    bool isInitialized() const { return m_initialized; }

    // Set usage pattern (will affect next setData call)
    void setUsage(GLenum usage) { m_usage = usage; }

    // Check if buffer is valid
    bool isValid() const {
        return m_bufferId != 0 && glIsBuffer(m_bufferId) == GL_TRUE;
    }

    // Get buffer parameter
    GLint getParameter(GLenum pname) const {
        bind();
        GLint result;
        glGetBufferParameteriv(m_target, pname, &result);
        return result;
    }

    // Copy data from another buffer
    void copyFrom(const GLBuffer& source, size_t readOffset = 0, size_t writeOffset = 0, size_t size = 0) {
        if (!source.isInitialized() || !m_initialized) {
            throw std::runtime_error("Both buffers must be initialized for copying");
        }
        
        if (size == 0) {
            size = std::min(source.getSize() - readOffset, m_size - writeOffset);
        }
        
        glBindBuffer(GL_COPY_READ_BUFFER, source.getID());
        glBindBuffer(GL_COPY_WRITE_BUFFER, m_bufferId);
        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, readOffset, writeOffset, size);
    }




};

// Convenience typedefs for common buffer types
using VertexBuffer = GLBuffer;
using IndexBuffer = GLBuffer;
using UniformBuffer = GLBuffer;
using ShaderStorageBuffer = GLBuffer;
// Helper functions for creating specific buffer types
inline GLBuffer createVertexBuffer(GLenum usage = GL_STATIC_DRAW) {
    return GLBuffer(GL_ARRAY_BUFFER, usage);
}

inline GLBuffer createIndexBuffer(GLenum usage = GL_STATIC_DRAW) {
    return GLBuffer(GL_ELEMENT_ARRAY_BUFFER, usage);
}

inline GLBuffer createUniformBuffer(GLenum usage = GL_STATIC_DRAW) {
    return GLBuffer(GL_UNIFORM_BUFFER, usage);
}
