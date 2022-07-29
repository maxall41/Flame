cmake_minimum_required(VERSION 3.22)
project(flame_engine)

set(CMAKE_CXX_STANDARD 14)

link_directories(/opt/homebrew/Cellar/sdl2/2.0.22/lib/)
link_directories(/opt/homebrew/Cellar/sdl2_image/2.6.0/lib)
link_directories(/opt/homebrew/Cellar/sdl2_ttf/2.20.0/lib)
add_executable(flame_engine core.cpp graphics.cpp graphics.h core.h testing.cpp fonts.cpp fonts.h consts.h sound.cpp sound.h)

target_link_libraries(flame_engine SDL2)
target_link_libraries(flame_engine SDL2_image)
target_link_libraries(flame_engine SDL2_ttf)
target_link_libraries(flame_engine SDL2_mixer)
target_include_directories(flame_engine
        PRIVATE
        /opt/homebrew/Cellar/sdl2/2.0.22/include/SDL2)

