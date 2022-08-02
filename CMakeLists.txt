cmake_minimum_required(VERSION 3.22)
project(flame_engine)

set(CMAKE_CXX_STANDARD 14)

link_directories(/opt/homebrew/Cellar/sdl2/2.0.22/lib/)
link_directories(/opt/homebrew/Cellar/sdl2_image/2.6.0/lib)
link_directories(/opt/homebrew/Cellar/sdl2_ttf/2.20.0/lib)
set(SOURCE_FILES core.cpp graphics.cpp sound.cpp fonts.cpp)
add_library(flame_engine ${SOURCE_FILES})
include(GNUInstallDirs)

set_target_properties(flame_engine PROPERTIES VERSION 1.0.0)
set_target_properties(flame_engine PROPERTIES PUBLIC_HEADER include/core.h include/fonts.h include/sound.h include/graphics.h include/consts.h)

target_link_libraries(flame_engine SDL2)
target_link_libraries(flame_engine SDL2_image)
target_link_libraries(flame_engine SDL2_ttf)
target_link_libraries(flame_engine SDL2_mixer)
target_include_directories(flame_engine
        PRIVATE
        /opt/homebrew/Cellar/sdl2/2.0.22/include/SDL2)
