#include "GL_Loader.hpp"
#include "Shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

float alpha = 0.5f;
const float alpha_increase = 0.005f;

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

    // Enabling Z-buffer
    glEnable( GL_DEPTH_TEST );

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
    uint32_t vbo, vao;  // vbo: vertex buffer object ; vao: vertex array object
    glGenBuffers( 1, &vbo );
    glGenVertexArrays( 1, &vao );

    float vertices[] = {
    //  Position             TextCoord
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    glBindVertexArray( vao ); // BEGIN RECORD VERTEX ATTRIBUTE

    // -- Vertex Buffer
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );

    // -- Texture
    unsigned int containerTexture;
    loadTexture( std::string(ASSETH_PATH) + "/container.jpg", GL_RGB, GL_CLAMP_TO_EDGE, GL_NEAREST, containerTexture );
    unsigned int awesomefaceTexture;
    loadTexture( std::string(ASSETH_PATH) + "/awesomeface.png", GL_RGBA, GL_MIRRORED_REPEAT, GL_LINEAR, awesomefaceTexture );

    // -- Change value in shader
    shader.use();
    shader.setInt("containerTex", 0);
    shader.setInt("awesomefaceTex", 1);
    shader.notUsed();

    unsigned int stride = 5 * sizeof(float);

    unsigned int position_index = 0;
    glVertexAttribPointer( position_index, 3, GL_FLOAT, GL_FALSE, stride, (void*)0 );
    glEnableVertexAttribArray( position_index );
    unsigned int texcoord_index = 1;
    glVertexAttribPointer( texcoord_index, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)) );
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
        float time = glfwGetTime();

        // input
        processInput( window );

        /**
         * @brief Render
         */
        /// clearing the screen
        glClearColor( 0.2f, 0.3f, 0.6f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // ..+ Bindings +..
        // ----------------
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, containerTexture );
        glActiveTexture( GL_TEXTURE1 );
        glBindTexture( GL_TEXTURE_2D, awesomefaceTexture );

        glBindVertexArray( vao );
        // ----------------

        // ..+ Setting up per-frame value +..
        // ----------------------------------
        shader.use();
        // -- Shader value for all object
        shader.setFloat("alpha", alpha);
        // -- Transformation value for all object
        glm::mat4 view = glm::translate(glm::mat4(1.0f), -1.0f * glm::vec3(0.0f, 0.0f, 5.0f) );
        shader.setMat4fv("view", view, 1, false);
        glm::mat4 proj = glm::perspective((float)glm::radians(45.0f), float(SCR_WIDTH)/float(SCR_HEIGHT), 0.1f, 100.0f );
        shader.setMat4fv("proj", proj, 1, false);
        const size_t size = sizeof(cubePositions);
        // -- Transformation value for individual object
        for(int i = 0; i < cubePositions.size(); ++i)
        {
            const float timePerIterate = time;

            glm::mat4 model = glm::mat4(1.0f);

            model = glm::translate(model, cubePositions[i]);

            const float angle = 20.0f * (i+1); 
            model = glm::rotate(model, timePerIterate * (float)glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f) );

            shader.setMat4fv("model", model, 1, false);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // ----------------------------------

        // .. + swap buffers and poll the events + ..
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    /**
     * @brief Deallocate
     */
    glfwDestroyWindow(window);
    glDeleteBuffers( 1, &vbo );
    glDeleteTextures( 1, &containerTexture );
    glDeleteTextures( 1, &awesomefaceTexture );
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

    if( glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS )
    {
        if( alpha < 1.0f )
            alpha += alpha_increase;
    }
    if( glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS )
    {
        if( alpha > 0.0f )
            alpha -= alpha_increase;
    }
}
