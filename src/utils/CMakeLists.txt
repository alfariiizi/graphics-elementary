cmake_minimum_required(VERSION 3.15)

include_guard()

include( ${CMAKE_SOURCE_DIR}/external/glad/CMakeLists.txt )

project( utils )

message(STATUS "[+] Utils are called!")

add_library( gl_loader 
    GL_Loader.cpp 
    Shader.cpp
)

target_link_libraries( gl_loader 
    PUBLIC
        gladd 
)

target_include_directories( gl_loader
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)