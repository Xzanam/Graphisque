#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include "GLBuffer.h"


class GLVertexArray{
private:
    GLuint vao_id;
    std::vector<std::reference_wrapper<GLBuffer>> vbo_refs; // Store references to GLBuffers
    GLBuffer* ebo_ref; // Reference to element buffer
    bool is_valid;
    
public:
    // Default constructor
   GLVertexArray() : vao_id(0), ebo_ref(nullptr), is_valid(false) {
        glGenVertexArrays(1, &vao_id);
        if (vao_id == 0) {
            throw std::runtime_error("Failed to generate VAO");
        }
        is_valid = true;
    }
    
    // Destructor
    ~ GLVertexArray() {
        cleanup();
    }
    
    // Move constructor
    GLVertexArray(GLVertexArray&& other) noexcept 
        : vao_id(other.vao_id), vbo_refs(std::move(other.vbo_refs)), 
          ebo_ref(other.ebo_ref), is_valid(other.is_valid) {
        other.vao_id = 0;
        other.ebo_ref = nullptr;
        other.is_valid = false;
    }
    
    // Move assignment operator
    GLVertexArray& operator=(GLVertexArray&& other) noexcept {
        if (this != &other) {
            cleanup();
            vao_id = other.vao_id;
            vbo_refs = std::move(other.vbo_refs);
            ebo_ref = other.ebo_ref;
            is_valid = other.is_valid;
            
            other.vao_id = 0;
            other.ebo_ref = nullptr;
            other.is_valid = false;
        }
        return *this;
    }
    
    // Delete copy constructor and assignment operator
    GLVertexArray(const GLVertexArray&) = delete;
    GLVertexArray& operator=(const GLVertexArray&) = delete;
    
    // Bind the VAO
    void bind() const {
        if (!is_valid) {
            throw std::runtime_error("Attempting to bind invalid VAO");
        }
        glBindVertexArray(vao_id);
    }
    
    // Unbind the GLVertexArray
    void unbind() const {
        glBindVertexArray(0);
    }

        // Helper struct for vertex attribute specification
    struct VertexAttribute {
        GLuint index;
        GLint size;
        GLenum type;
        GLboolean normalized;
        GLsizei stride;
        size_t offset;
        
        VertexAttribute(GLuint idx, GLint sz, GLenum tp, 
                       GLboolean norm = GL_FALSE, GLsizei str = 0, size_t off = 0)
            : index(idx), size(sz), type(tp), normalized(norm), stride(str), offset(off) {}
    };
    
    // Add GLBuffer as vertex buffer and set up vertex attribute
    /** 
     * @breif Creates a VAO from provided VBO
     * 
     * @param buffer GLBuffer Object 
     * @param index  Specifies the index of the generic vertex attribute to be modified
     * @param size Specifies the number of components per generic vertex attrib
     * @param normalized Specifies whether fixed point data should be normalized
     * @param stride Specifies the byte offset between consecutive generic vertex attribute
    **/
    void addVertexBuffer(GLBuffer& buffer, GLuint index, GLint size, GLenum type, 
                        GLboolean normalized = GL_FALSE, GLsizei stride = 0, 
                        const void* pointer = nullptr) {
        if (!is_valid) {
            throw std::runtime_error("VAO is not valid");
        }
        
        if (buffer.getTarget() != GL_ARRAY_BUFFER) {
            throw std::runtime_error("Buffer must be an array buffer (GL_ARRAY_BUFFER)");
        }
        
        if (!buffer.isInitialized()) {
            throw std::runtime_error("Buffer must be initialized before adding to VAO");
        }
        
        bind();
        buffer.bind();
        
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        glEnableVertexAttribArray(index);
        
        // Store reference to the buffer
        vbo_refs.emplace_back(std::ref(buffer));
        
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            throw std::runtime_error("OpenGL error while setting up vertex attribute: " + std::to_string(error));
        }
    }
    
    // // Convenience method for common vertex attribute patterns
    // void addVertexBuffer(GLBuffer& buffer, GLuint index, GLint size, GLenum type = GL_FLOAT) {
    //     addVertexBuffer(buffer, index, size, type, GL_FALSE, 0, nullptr);
    // }
    
    // Add multiple vertex attributes from the same buffer (interleaved data)
    void addInterleavedVertexBuffer(GLBuffer& buffer, 
                                   const std::vector<VertexAttribute>& attributes) {
        if (!is_valid) {
            throw std::runtime_error("VAO is not valid");
        }
        
        if (buffer.getTarget() != GL_ARRAY_BUFFER) {
            throw std::runtime_error("Buffer must be an array buffer (GL_ARRAY_BUFFER)");
        }
        
        if (!buffer.isInitialized()) {
            throw std::runtime_error("Buffer must be initialized before adding to VAO");
        }
        
        bind();
        buffer.bind();
        
        for (const auto& attr : attributes) {
            glVertexAttribPointer(attr.index, attr.size, attr.type, attr.normalized, 
                                attr.stride, reinterpret_cast<const void*>(attr.offset));
            glEnableVertexAttribArray(attr.index);
        }
        
        // Store reference to the buffer
        vbo_refs.emplace_back(std::ref(buffer));
        
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            throw std::runtime_error("OpenGL error while setting up interleaved vertex attributes: " + std::to_string(error));
        }
    }
    
    // Set element buffer (index buffer)
    void setElementBuffer(GLBuffer& buffer) {
        if (!is_valid) {
            throw std::runtime_error("VAO is not valid");
        }
        
        if (buffer.getTarget() != GL_ELEMENT_ARRAY_BUFFER) {
            throw std::runtime_error("Buffer must be an element buffer (GL_ELEMENT_ARRAY_BUFFER)");
        }
        
        if (!buffer.isInitialized()) {
            throw std::runtime_error("Buffer must be initialized before setting as element buffer");
        }
        
        bind();
        buffer.bind();
        
        ebo_ref = &buffer;
        
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            throw std::runtime_error("OpenGL error while setting element buffer: " + std::to_string(error));
        }
    }
    
    // Enable vertex attribute array
    void enableVertexAttribArray(GLuint index) {
        if (!is_valid) {
            throw std::runtime_error("VAO is not valid");
        }
        
        bind();
        glEnableVertexAttribArray(index);
    }
    
    // Disable vertex attribute array
    void disableVertexAttribArray(GLuint index) {
        if (!is_valid) {
            throw std::runtime_error("VAO is not valid");
        }
        
        bind();
        glDisableVertexAttribArray(index);
    }
    
    // Draw arrays
    void drawArrays(GLenum mode, GLint first, GLsizei count) const {
        if (!is_valid) {
            throw std::runtime_error("VAO is not valid");
        }
        
        bind();
        glDrawArrays(mode, first, count);
        
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            throw std::runtime_error("OpenGL error during drawArrays: " + std::to_string(error));
        }
    }
    
    // Draw elements
    void drawElements(GLenum mode, GLsizei count, GLenum type, const void* indices = nullptr) const {
        if (!is_valid) {
            throw std::runtime_error("VAO is not valid");
        }
        
        if (!ebo_ref && indices == nullptr) {
            throw std::runtime_error("No element buffer set and no indices provided");
        }
        
        bind();
        glDrawElements(mode, count, type, indices);
        
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            throw std::runtime_error("OpenGL error during drawElements: " + std::to_string(error));
        }
    }
    
    // Draw elements with automatic count calculation
    void drawElements(GLenum mode, GLenum type = GL_UNSIGNED_INT) const {
        if (!ebo_ref) {
            throw std::runtime_error("No element buffer set for automatic draw");
        }
        
        // Calculate count based on buffer size and type
        size_t typeSize = getIndexTypeSize(type);
        GLsizei count = static_cast<GLsizei>(ebo_ref->getSize() / typeSize);
        
        drawElements(mode, count, type);
    }
    
    // Get VAO ID
    GLuint getId() const {
        return vao_id;
    }
    
    // Check if VAO is valid
    bool isValid() const {
        return is_valid;
    }
    
    // Get number of VBOs
    size_t getVBOCount() const {
        return vbo_refs.size();
    }
    
    // Check if element buffer is set
    bool hasElementBuffer() const {
        return ebo_ref != nullptr;
    }
    
    // Get element buffer reference (returns nullptr if not set)
    GLBuffer* getElementBuffer() const {
        return ebo_ref;
    }
    
    // Clear all buffer references (doesn't delete the buffers)
    void clearBufferReferences() {
        vbo_refs.clear();
        ebo_ref = nullptr;
    }
    
    // Check OpenGL version compatibility
    static bool checkOpenGLVersion() {
        GLint major, minor;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        
        // Ensure we have at least OpenGL 3.3
        return (major > 3) || (major == 3 && minor >= 3);
    }
    
    // Get OpenGL version string
    static std::string getOpenGLVersion() {
        const GLubyte* version = glGetString(GL_VERSION);
        return version ? std::string(reinterpret_cast<const char*>(version)) : "Unknown";
    }
    

    
private:
    void cleanup() {
        if (is_valid && vao_id != 0) {
            glDeleteVertexArrays(1, &vao_id);
            vao_id = 0;
        }
        
        // Clear references (but don't delete the actual buffers)
        vbo_refs.clear();
        ebo_ref = nullptr;
        is_valid = false;
    }
    
    // Helper function to get size of index types
    size_t getIndexTypeSize(GLenum type) const {
        switch (type) {
            case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
            case GL_UNSIGNED_SHORT: return sizeof(GLushort);
            case GL_UNSIGNED_INT: return sizeof(GLuint);
            default: return sizeof(GLuint); // Default fallback
        }
    }
};

// Helper class for managing vertex layouts with GLBuffer integration
class VertexLayout {
public:
    using Attribute = GLVertexArray::VertexAttribute;
    
private:
    std::vector<Attribute> attributes;
    GLsizei total_stride;
    
public:
    VertexLayout() : total_stride(0) {}
    
    // Add attribute to layout
    void addAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized = GL_FALSE) {
        attributes.emplace_back(index, size, type, normalized);
        calculateStride();
    }
    
    // Apply layout to VAO with GLBuffer
    void applyToVAO(GLVertexArray& vao, GLBuffer& buffer) const {
        if (attributes.empty()) {
            throw std::runtime_error("No attributes defined in layout");
        }
        
        vao.addInterleavedVertexBuffer(buffer, attributes);
    }
    
    // Get total stride
    GLsizei getStride() const {
        return total_stride;
    }
    
    // Get attributes
    const std::vector<Attribute>& getAttributes() const {
        return attributes;
    }
    
    // Clear all attributes
    void clear() {
        attributes.clear();
        total_stride = 0;
    }
    
private:
    void calculateStride() {
        total_stride = 0;
        size_t current_offset = 0;
        
        for (auto& attr : attributes) {
            attr.offset = current_offset;
            
            size_t type_size = getTypeSize(attr.type);
            size_t attr_size = attr.size * type_size;
            
            current_offset += attr_size;
            total_stride = static_cast<GLsizei>(current_offset);
        }
        
        // Update stride for all attributes
        for (auto& attr : attributes) {
            attr.stride = total_stride;
        }
    }
    
    size_t getTypeSize(GLenum type) const {
        switch (type) {
            case GL_BYTE:
            case GL_UNSIGNED_BYTE:
                return sizeof(GLbyte);
            case GL_SHORT:
            case GL_UNSIGNED_SHORT:
                return sizeof(GLshort);
            case GL_INT:
            case GL_UNSIGNED_INT:
                return sizeof(GLint);
            case GL_FLOAT:
                return sizeof(GLfloat);
            case GL_DOUBLE:
                return sizeof(GLdouble);
            default:
                return sizeof(GLfloat); // Default fallback
        }
    }
};

using VerteXArray = GLVertexArray;

#endif // VAO_H