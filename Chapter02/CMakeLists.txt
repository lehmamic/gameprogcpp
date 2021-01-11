cmake_minimum_required(VERSION 3.17)
project(Chapter02)

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
        VERSION 2.0.14
        URL https://libsdl.org/release/SDL2-2.0.14.zip
)

if (SDL2_ADDED)
    add_library(SDL2::SDL2 ALIAS SDL2)
endif()

CPMAddPackage(
        NAME SDL2_image
        VERSION 2.0.5
        URL https://libsdl.org/projects/SDL_image/release/SDL2_image-2.0.5.zip
        DOWNLOAD_ONLY TRUE
)

if (SDL2_image_ADDED)


    find_package(JPEG REQUIRED)
    set(LINK_LIBS "${LINK_LIBS}" "${JPEG_LIBRARIES}")
    set(INCLUDE_DIRS "${INCLUDE_DIRS}" "${JPEG_INCLUDE_DIRS}")

    set(ENABLE_DEFS "${ENABLE_DEFS}" LOAD_PNG)
    find_package(PNG REQUIRED)
    set(LINK_LIBS "${LINK_LIBS}" "${PNG_LIBRARIES}")
    set(INCLUDE_DIRS "${INCLUDE_DIRS}" "${PNG_INCLUDE_DIRS}")

    set(ENABLE_DEFS "${ENABLE_DEFS}" LOAD_TIF)
    find_package(TIFF REQUIRED)
    set(LINK_LIBS "${LINK_LIBS}" "${TIFF_LIBRARIES}")
    set(INCLUDE_DIRS "${INCLUDE_DIRS}" "${TIFF_INCLUDE_DIRS}")

    set(ENABLE_DEFS "${ENABLE_DEFS}" LOAD_WEBP)
    find_package(WebP REQUIRED) # BROKEN, debian doesn't install the cmake config files, so find_package will fail
    set(LINK_LIBS "${LINK_LIBS}" "${WebP_LIBRARIES}")
    set(INCLUDE_DIRS "${INCLUDE_DIRS}" "${WebP_INCLUDE_DIRS}")

    # quick and dirty target definitions
    # might need extra configuration and install commands to work properly
    file(GLOB SDL2_image_SOURCES "${SDL2_image_SOURCE_DIR}/*.c")
    add_library(SDL2_image ${SDL2_image_SOURCES})
    target_link_libraries(SDL2_image SDL2::SDL2)
    target_include_directories(SDL2_image PUBLIC ${SDL2_image_SOURCE_DIR})
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

add_executable(Chapter02 main.cpp Actor.cpp Actor.h Component.cpp Component.h Game.cpp Game.h Math.cpp Math.h)

target_include_directories(Chapter02 PUBLIC PUBLIC $<BUILD_INTERFACE:${SDL2_image_SOURCE_DIR}>)
target_include_directories(Chapter02 PUBLIC PUBLIC $<BUILD_INTERFACE:${SDL2_ttf_SOURCE_DIR}>)
target_include_directories(Chapter02 PUBLIC PUBLIC $<BUILD_INTERFACE:${SDL2_mixer_SOURCE_DIR}>)
target_include_directories(Chapter02 PUBLIC PUBLIC $<BUILD_INTERFACE:${SDL2_SOURCE_DIR}/include>)

target_link_libraries(Chapter02 SDL2  SDL2_ttf SDL2_image SDL2_mixer)