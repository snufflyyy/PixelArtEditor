cmake_minimum_required(VERSION 3.30)
project(PixelArtEditor)

add_subdirectory(external/raylib)

include_directories(
    include
    external/raylib/include
)

add_executable(
    ${PROJECT_NAME}
    src/main.c
    src/application.c
    src/mainmenu.c
    src/editor.c
)

target_link_libraries(${PROJECT_NAME} raylib)
