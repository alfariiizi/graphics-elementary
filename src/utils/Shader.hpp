#pragma once

#include "GL_Loader.hpp"

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
    unsigned int getId() const;

private:
    bool readShader( const std::string& filename, std::string& outShader );
    bool loadShader( const std::string& filename, GLenum shaderType, GLuint& outShader );

private:
    unsigned int id;
};