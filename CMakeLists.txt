project(boring_game)

cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON) # -std=c++11 instead of -std=gnu++11
set(MY_CXX_COMPILE_FEATURES cxx_generic_lambdas cxx_range_for cxx_strong_enums)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/build)

file(GLOB SOURCE src/*.cpp)
file(GLOB HEADER include/*.h)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(LibXml2 REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/lib
    ${JSONCPP_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${LIBXML2_INCLUDE_DIR})

add_executable(boring_game ${SOURCE} ${HEADER})

target_link_libraries(
    boring_game 
    ${OPENGL_LIBRARY}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
    ${LIBXML2_LIBRARIES}
    ${JSONCPP_LIBRARIES}
)

