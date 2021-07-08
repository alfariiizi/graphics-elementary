#pragma once

#include "GL_Loader.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Shader
{
public:
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    void destroy();
    void use() const;
    void notUsed() const;
    void setBool( const std::string& name, bool value );
    void setInt( const std::string&name, int value );
    void setFloat( const std::string&name, float value );
    void setMat4fv( const std::string& name, const glm::mat4& matrix, int matrixSize, bool transpose );
    unsigned int getId() const;

private:
    bool readShader( const std::string& filename, std::string& outShader );
    bool loadShader( const std::string& filename, GLenum shaderType, GLuint& outShader );

private:
    unsigned int id;
};