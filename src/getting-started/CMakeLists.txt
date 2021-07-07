cmake_minimum_required( VERSION 3.10 )

include_guard()

message( STATUS "[+] Getting-started are called!")

project( getting-started )

add_definitions(-DSHADER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/shaders")
add_definitions(-DASSETH_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets")

set( srcFile ${CMAKE_CURRENT_SOURCE_DIR})

set( CMAKE_BUILD_TYPE "Debug" )

add_executable( get-start
    ${srcFile}/main.cpp
)

# include( ${CMAKE_SOURCE_DIR}/external/glad/CMakeLists.txt )

# add_subdirectory( ${CMAKE_SOURCE_DIR}/external/glad )
# message( "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}" )
# message( "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries( get-start 
    PRIVATE
        gl_loader
)

# target_include_directories( get-start
#     ${CMAKE_SOURCE_DIR}/external/glad/
# )