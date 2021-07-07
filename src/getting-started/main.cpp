#include "GL_Loader.hpp"
#include "Shader.hpp"
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <vector>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600


#define CHECK( boolean, msg ) if( boolean ) ERR_MESSAGE(msg);

#ifndef SHADER_PATH
    #define SHADER_PATH ""
#endif

void framebufferSizeCallback( GLFWwindow* window, int width, int height );
void processInput( GLFWwindow* window );

int main()
{
    GLFWwindow* window = gload::load(SCR_WIDTH, SCR_HEIGHT, "Getting Started");
    if( window == nullptr )
        return EXIT_FAILURE;

    glfwSetFramebufferSizeCallback( window, framebufferSizeCallback );


    /**
     * @brief Creating shaders
     */

    /// Creating shaders
    const std::string shaderPath = SHADER_PATH;
    Shader shader{shaderPath + "/vertexShader.vert", shaderPath + "/fragmentShader.frag"};


    /**
     * @brief Create Vertex Buffer
     * 
     */
    uint32_t vbo, vao, ebo;  // vbo: vertex buffer object ; vao: vertex array object
    glGenBuffers( 1, &vbo );
    glGenBuffers( 1, &ebo );
    glGenVertexArrays( 1, &vao );

    float vertices [] = {
        // position         // color
        0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // top right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f // top left 
    };
    unsigned int indices [] = {
        0, 1, 3,
        1, 2, 3
        // so, it's clockwise
    };

    glBindVertexArray( vao ); // BEGIN RECORD VERTEX ATTRIBUTE

    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW );

    unsigned int position_index = 0;
    glVertexAttribPointer( position_index, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0 );
    glEnableVertexAttribArray( position_index );
    unsigned int color_index = 1;
    glVertexAttribPointer( color_index, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)) );
    glEnableVertexAttribArray( color_index );

    glBindVertexArray( 0 ); // END RECORD

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer( GL_ARRAY_BUFFER, 0 ); // it can be placed before/after glBindVertexArray(0)

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ); // So, if it's placed before glBindVertexArray(0), it'll do NOT work


    /**
     * @brief Render Looping
     */
    while( !glfwWindowShouldClose(window) )
    {
        // input
        processInput( window );

        /**
         * @brief Render
         */
        /// clearing the screen
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );

        // ..+ Setting up per-frame value +..
        // ----------------------------------
        shader.use();

        float timeValue = glfwGetTime();
        float u_color = glm::sin( timeValue ) / 2.0f + 0.5f;
        shader.setFloat( "u_colorChangeable", u_color );
        // ----------------------------------

        /// draw the triangle
        glBindVertexArray( vao );
        // glDrawArrays( GL_TRIANGLES, 0, 3 );
        glDrawElements( GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, (void*)0 );

        // swap buffers and poll the events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    /**
     * @brief Deallocate
     */
    glfwDestroyWindow(window);
    glDeleteBuffers( 1, &vbo );
    glDeleteVertexArrays( 1, &vao );

    return EXIT_SUCCESS;
}


void framebufferSizeCallback( GLFWwindow* window, int width, int height )
{
    glViewport( 0, 0, width, height );
}

void processInput( GLFWwindow* window )
{
    if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS )
    {
        glfwSetWindowShouldClose( window, true );
    }
    if( glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS )
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }
    else
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
}
