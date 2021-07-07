#pragma once

#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#define ERR_MESSAGE( str ) std::cerr << "FAILED: " << str << '\n';

namespace gload
{
    GLFWwindow* load( int screenWidth, int screenHeight, const std::string& windowName );
    void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                                const char* message, const void* userParam );
} // namespace gload
