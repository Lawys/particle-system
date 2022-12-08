#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include "Math.hpp"

class Shader
{
public:
    unsigned int ID = 0;

    Shader() {};
    ~Shader() {};

    void Create();

    void setInt(const std::string& name, const int& value) const;
    void setFloat(const std::string& name, const float& value) const;
    void setVec3(const std::string& name, const vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setMat4(const std::string& name, const mat4& mat) const;
private:
    void checkCompileErrors(GLuint shader, std::string type);
};
#endif
