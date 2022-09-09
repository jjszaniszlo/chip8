cmake_minimum_required(VERSION 3.24.0)

project(chip8C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_INSTALL_PREFIX ..)

add_executable(chip8C src/glad.c src/main.c 
                      src/cpu.c src/render.c 
                      src/io.c
)

include_directories(chip8C include /opt/homebrew/include)

find_package(glfw3 REQUIRED)
target_link_libraries(chip8C glfw)

install(TARGETS chip8C RUNTIME DESTINATION bin)
