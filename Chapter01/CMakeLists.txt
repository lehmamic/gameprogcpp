cmake_minimum_required(VERSION 3.17)
project(Chapter01)

set(CMAKE_CXX_STANDARD 14)

# ---- Fetch CPM ----

set(CPM_DOWNLOAD_VERSION 0.27.1)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD
            https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
            ${CPM_DOWNLOAD_LOCATION}
            )
endif()

include(${CPM_DOWNLOAD_LOCATION})

# ---- Add dependencies via CPM ----
# See https://github.com/TheLartians/CPM.cmake for details and examples

CPMAddPackage(
        NAME SDL2
        VERSION 2.0.12
        URL https://libsdl.org/release/SDL2-2.0.12.zip
)

if (SDL2_ADDED)
    add_library(SDL2::SDL2 ALIAS SDL2)
endif()

CPMAddPackage(
        NAME SDL2_ttf
        VERSION 2.0.15
        URL https://libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.15.zip
)

CPMAddPackage(
        NAME SDL2_mixer
        VERSION 2.0.15
        URL https://libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.0.4.zip
        DOWNLOAD_ONLY TRUE
)

if (SDL2_mixer_ADDED)
    # quick and dirty target definitions
    # might need extra configuration and install commands to work properly
    file(GLOB SDL2_mixer_SOURCES "${SDL2_mixer_SOURCE_DIR}/*.c")
    add_library(SDL2_mixer ${SDL2_mixer_SOURCES})
    target_link_libraries(SDL2_mixer SDL2::SDL2)
    target_include_directories(SDL2_mixer PUBLIC ${SDL2_mixer_SOURCE_DIR})
endif()

# ---- Create executable target ----

add_executable(Chapter01 main.cpp Game.cpp Game.h)

target_include_directories(Chapter01 PUBLIC PUBLIC $<BUILD_INTERFACE:${SDL2_ttf_SOURCE_DIR}>)
target_include_directories(Chapter01 PUBLIC PUBLIC $<BUILD_INTERFACE:${SDL2_mixer_SOURCE_DIR}>)
target_include_directories(Chapter01 PUBLIC PUBLIC $<BUILD_INTERFACE:${SDL2_SOURCE_DIR}/include>)

target_link_libraries(Chapter01 SDL2 SDL2_ttf SDL2_mixer)
