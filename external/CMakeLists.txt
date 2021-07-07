cmake_minimum_required( VERSION 3.10 )

# .+ Glad +.
# ----------

message( STATUS "[+] External dependencies are called!")

add_library( gladd 
    ${CMAKE_CURRENT_LIST_DIR}/glad/glad.c
)

target_link_libraries( gladd
    PUBLIC
        GL
        glfw
        X11
        pthread
        Xrandr
        Xi
        dl
)

target_include_directories( gladd
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/glad
)
# ----------


# .+ STB +.
# ---------

add_library( stb_image 
    ${CMAKE_CURRENT_LIST_DIR}/stb/stb_image.c
)

target_include_directories( stb_image
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/stb
)
# ---------

