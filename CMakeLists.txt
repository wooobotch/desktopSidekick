cmake_minimum_required(VERSION 3.10)
project(mascota_sdl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/embedded
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
)

message(STATUS "SDL2 include dirs: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 libs: ${SDL2_LIBRARIES}")
message(STATUS "SDL2_image include dirs: ${SDL2_IMAGE_INCLUDE_DIRS}")
message(STATUS "SDL2_image libs: ${SDL2_IMAGE_LIBRARIES}")

file(GLOB ASSET_IMAGES assets/*.png)

set(EMBEDDED_HEADERS "")
foreach(png ${ASSET_IMAGES})
    get_filename_component(base "${png}" NAME)
    string(REGEX REPLACE "[^a-zA-Z0-9]" "_" name "${base}")
    string(TOLOWER "${name}" name)
    list(APPEND EMBEDDED_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/embedded/${name}.h")
endforeach()

add_custom_command(
    OUTPUT ${EMBEDDED_HEADERS}
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/png2hex.sh ${ASSET_IMAGES}
    DEPENDS ${ASSET_IMAGES} png2hex.sh
    COMMENT "Generando headers embebidos desde PNGs"
)

add_custom_target(embed_assets DEPENDS ${EMBEDDED_HEADERS})

add_executable(mascota_sdl main.cpp ${EMBEDDED_HEADERS})
add_dependencies(mascota_sdl embed_assets)


target_link_libraries(mascota_sdl
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    X11 Xext Xfixes
    SDL2_image
)
