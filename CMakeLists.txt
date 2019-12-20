project(kusoge)

cmake_minimum_required(VERSION 3.9)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON) # -std=c++11 instead of -std=gnu++11
set(MY_CXX_COMPILE_FEATURES cxx_generic_lambdas cxx_range_for cxx_strong_enums)
set(JSON_BuildTests OFF CACHE INTERNAL "")


set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)

file(GLOB_RECURSE SOURCE src/*.cpp)
file(GLOB_RECURSE HEADER include/*.h)

find_package(nlohmann_json REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2Mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/libs
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2Mixer_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
    )

add_executable(kusoge ${SOURCE} ${HEADER})

target_link_libraries( 
    kusoge 
    PRIVATE nlohmann_json::nlohmann_json
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2Mixer_LIBRARY}
    ${SDL2_TTF_LIBRARIES}
)
