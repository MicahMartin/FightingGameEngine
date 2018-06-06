project(boring_game)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/build)

file(GLOB SOURCE src/*.cpp)
file(GLOB HEADER src/*.h)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(LibXml2 REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${OPENGL_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${LIBXML2_INCLUDE_DIR})

add_executable(boring_game ${SOURCE} ${HEADER})

target_link_libraries(
    boring_game 
    ${OPENGL_LIBRARY}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
    ${LIBXML2_LIBRARIES})
