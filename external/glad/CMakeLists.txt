cmake_minimum_required( VERSION 3.10 )
message( STATUS "[+] Glad are called!")

add_library( gladd 
    ${CMAKE_CURRENT_LIST_DIR}/glad.c
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
        ${CMAKE_CURRENT_LIST_DIR}
)
