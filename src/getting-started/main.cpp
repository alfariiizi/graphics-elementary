#include "GL_Loader.hpp"
#include "Shader.hpp"

#include <glm/glm.hpp>

#include "stb_image.h"

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
void loadTexture( const std::string& pathToImage, unsigned int imageFormat, int wrap, int filtering, unsigned int& texture )
{
    glGenTextures(1, &texture);
    glBindTexture( GL_TEXTURE_2D, texture );

    // wrap_s and wrap_t don't have to have the same value. So it could be hybrid (combination of other wrapping)
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering );

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load( true );
    unsigned char* data = stbi_load(pathToImage.c_str(), &width, &height, &nrChannels, 0 );
    if( data )
    {
        glTexImage2D( GL_TEXTURE_2D, 0, imageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data );
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to Load The Texture\n";
    }
    stbi_image_free( data );
    glBindTexture( GL_TEXTURE_2D, 0 );
}

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

    // topright - bottomleft = width and height. So the more range between bottomleft and topright, the more width and height it get.
    glm::vec2 bottomleft = { 0.3f, 0.3f };
    glm::vec2 topright = { 0.7f, 0.7f };
    // // Try use this below, we'll see that the image is getting more zoom than before.
    // glm::vec2 bottomleft = { 0.45f, 0.45f };
    // glm::vec2 topright = { 0.55f, 0.55f };

    float vertices [] = {
        // position         // color            // texcoord
        0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   topright.x, topright.y, // top right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   topright.x, bottomleft.y,   // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   bottomleft.x, bottomleft.y, // bottom left
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f,   bottomleft.x, topright.y    // top left 
    };
    unsigned int indices [] = {
        0, 1, 3,
        1, 2, 3
        // so, it's clockwise
    };

    glBindVertexArray( vao ); // BEGIN RECORD VERTEX ATTRIBUTE

    // -- Vertex Buffer
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

    // -- Element Buffer (index buffer)
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW );

    // -- Texture
    unsigned int containerTexture;
    loadTexture( std::string(ASSETH_PATH) + "/container.jpg", GL_RGB, GL_CLAMP_TO_EDGE, GL_NEAREST, containerTexture );
    unsigned int awesomefaceTexture;
    loadTexture( std::string(ASSETH_PATH) + "/awesomeface.png", GL_RGBA, GL_MIRRORED_REPEAT, GL_LINEAR, awesomefaceTexture );

    shader.use();
    shader.setInt("containerTex", 0);
    shader.setInt("awesomefaceTex", 1);
    shader.notUsed();

    unsigned int stride = 8 * sizeof(float);

    unsigned int position_index = 0;
    glVertexAttribPointer( position_index, 3, GL_FLOAT, GL_FALSE, stride, (void*)0 );
    glEnableVertexAttribArray( position_index );
    unsigned int color_index = 1;
    glVertexAttribPointer( color_index, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)) );
    glEnableVertexAttribArray( color_index );
    unsigned int texcoord_index = 2;
    glVertexAttribPointer( texcoord_index, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)) );
    glEnableVertexAttribArray( texcoord_index );

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
        glClearColor( 0.2f, 0.3f, 0.6f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );

        // ..+ Setting up per-frame value +..
        // ----------------------------------
        shader.use();

        // float timeValue = glfwGetTime();
        // float u_color = glm::sin( timeValue ) / 2.0f + 0.5f;
        // shader.setFloat( "u_colorChangeable", u_color );
        // ----------------------------------

        // ..+ Bindings +..
        // ----------------
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, containerTexture );
        glActiveTexture( GL_TEXTURE1 );
        glBindTexture( GL_TEXTURE_2D, awesomefaceTexture );

        glBindVertexArray( vao );
        // ----------------

        /// draw the triangle
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
    glDeleteBuffers( 1, &ebo );
    glDeleteTextures( 1, &containerTexture );
    glDeleteVertexArrays( 1, &vao );
    shader.destroy();

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
