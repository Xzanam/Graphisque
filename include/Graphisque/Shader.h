#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
public:
    unsigned int ID;
    Shader() = default;
    Shader(const char *vertexPath, const char *fragmentPath);
    // Shader(const char *vertexPath, const char *includePath, const char *fragmentPath);

    void use() const ;

    void setBool(const std::string &name, bool value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setInt(const std::string &name, int value) const;
};