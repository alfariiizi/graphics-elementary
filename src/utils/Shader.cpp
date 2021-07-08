#include "Shader.hpp"
#include <fstream>
#include <vector>
#include <assert.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    unsigned int vertexShader;
    loadShader(vertexShaderPath, GL_VERTEX_SHADER, vertexShader);
    unsigned int fragmentShader;
    loadShader(fragmentShaderPath, GL_FRAGMENT_SHADER, fragmentShader);

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    int success;
    glGetProgramiv( id, GL_LINK_STATUS, &success );
    assert( success );

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::destroy() 
{
    glDeleteProgram( id );
}

void Shader::use() const
{
    glUseProgram(id);
}

void Shader::notUsed() const
{
    glUseProgram( 0 );
}

void Shader::setBool(const std::string& name, bool value) 
{
    glUniform1i( glGetUniformLocation(id, name.c_str()), int(value) );
}

void Shader::setInt(const std::string&name, int value) 
{
    glUniform1i( glGetUniformLocation(id, name.c_str()), value );
}

void Shader::setFloat(const std::string&name, float value) 
{
    glUniform1f( glGetUniformLocation(id, name.c_str()), value );
}

void Shader::setMat4fv(const std::string& name, const glm::mat4& matrix, int matrixSize, bool transpose) 
{
    glUniformMatrix4fv( glGetUniformLocation(id, name.c_str()), matrixSize, transpose, glm::value_ptr(matrix) );
}

unsigned int Shader::getId() const
{
    return id;
}

bool Shader::readShader( const std::string& filename, std::string& outShader )
{
    std::ifstream in( filename );
    if( in.eof() )
    {
        in.close();
        return false;
    }

    // in.seekg(0, std::ios_base::end);
    // auto endPos = in.tellg();
    // in.seekg(0, std::ios_base::beg);

    std::string reader;
    std::string buff;
    // std::string buff2;

    std::vector<std::string> vbuff;
    while( !in.eof() )
    {
        std::getline( in, buff, '\n' );
        vbuff.emplace_back( buff );
    }
    in.close();

    for( auto it = vbuff.begin(); it != vbuff.end(); ++it )
    {
        reader.append(*it);
        if( it + 1 == vbuff.end() || it->empty() )
        {
            continue;
        }
        reader.append("\n");
    }
    // while( !in.eof() )
    // {
    //     std::getline( in, buff, '\n' );
    //     if( buff.empty() || in.tellg() == endPos ) continue;
    //     buff.append("\n");
    //     reader.append( buff );
    //     buff.clear();
    // }

    outShader = std::move(reader);
    // outShader.append("\0");

    return true;
}

bool Shader::loadShader( const std::string& filename, GLenum shaderType, GLuint& outShader )
{
    std::string shaderFile;
    bool read = readShader(filename, shaderFile);
    if( !read )
    {
        ERR_MESSAGE("Read the shader");
        return false;
    }

    GLuint shader;
    const char* shaderSource = shaderFile.c_str();

    shader = glCreateShader( shaderType );
    glShaderSource( shader, 1, &shaderSource, nullptr );
    glCompileShader( shader );

    int success;
    char infoLog[512];
    glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
    if( !success )
    {
        glGetShaderInfoLog( shader, sizeof(infoLog), nullptr, infoLog );
        ERR_MESSAGE(infoLog);
        return false;
    }

    outShader = std::move( shader );
    return true;
}