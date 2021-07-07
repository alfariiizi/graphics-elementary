cmake_minimum_required( VERSION 3.10 )

set( CMAKE_CXX_STANDARD 17 )

project( graphics-elementary )

add_subdirectory( src/utils )

# add_subdirectory( ${CMAKE_SOURCE_DIR}/external/glad )

# ..++ Getting Started ++..
add_subdirectory( src/getting-started )

set( BUILD_SHARED_LIBS)