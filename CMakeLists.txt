cmake_minimum_required(VERSION 3.10)
project(mascota_sdl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)

message(STATUS "SDL2 include dirs: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 libs: ${SDL2_LIBRARIES}")
message(STATUS "SDL2_image include dirs: ${SDL2_IMAGE_INCLUDE_DIRS}")
message(STATUS "SDL2_image libs: ${SDL2_IMAGE_LIBRARIES}")

add_executable(mascota_sdl main.cpp)

target_link_libraries(mascota_sdl
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    X11 Xext Xfixes
    SDL2_image
)
