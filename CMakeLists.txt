cmake_minimum_required(VERSION 3.22)
project(flame_engine)

set(CMAKE_CXX_STANDARD 14)

link_directories(/opt/homebrew/Cellar/sdl2/2.0.22/lib/)

add_executable(flame_engine main.cpp)

target_link_libraries(flame_engine SDL2)
target_link_libraries(flame_engine SDL2_gpu)

target_include_directories(flame_engine
        PRIVATE
        /opt/homebrew/Cellar/sdl2/2.0.22/include/)

