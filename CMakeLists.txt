cmake_minimum_required(VERSION 3.28)
project(cubepp)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG ba2f78a0069118a6c583f1fbf1420144ffa35bad
)

FetchContent_Declare(
        SDL2_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG 4a318f8dfaa1bb6f10e0c5e54052e25d3c7f3440
)

FetchContent_Declare(
        vk_bootstrap
        GIT_REPOSITORY  https://github.com/charles-lunarg/vk-bootstrap.git
        GIT_TAG         217f5a9101c23772c0632039ac5910bb14fc57ce
)

if (WIN32 OR APPLE)
    message("Downloading dependencies...")
    FetchContent_MakeAvailable(SDL2 SDL2_ttf)
else ()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(Vulkan REQUIRED)
endif ()

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${Vulkan_INCLUDE_DIRS})

FetchContent_MakeAvailable(vk_bootstrap)

message(STATUS "SDL2 version: " ${SDL2_VERSION})
message(STATUS "SDL2_ttf version: " ${SDL2_ttf_VERSION})
message(STATUS "Vulkan version: " ${Vulkan_VERSION})

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

add_executable(cubepp src/main.cpp src/game.cpp src/mainmenu_scene.cpp src/ingame_scene.cpp src/demo_scene.cpp)
target_link_libraries(cubepp
        PRIVATE
        SDL2::SDL2
        SDL2_ttf::SDL2_ttf
        Vulkan::Vulkan
        vk-bootstrap::vk-bootstrap
)
