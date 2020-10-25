project(kusoge)

cmake_minimum_required(VERSION 3.9)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-no_pie,-export_dynamic")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON) # -std=c++11 instead of -std=gnu++11
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
set(MY_CXX_COMPILE_FEATURES cxx_generic_lambdas cxx_range_for cxx_strong_enums)
set(JSON_BuildTests OFF CACHE INTERNAL "")

file(GLOB_RECURSE SOURCE src/*.cpp)
file(GLOB_RECURSE HEADER include/*.h)

find_package(nlohmann_json REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2Mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Boost 1.55 REQUIRED system serialization)
find_package(Gperftools REQUIRED COMPONENTS TCMALLOC_AND_PROFILER)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/libs
    ${PROJECT_SOURCE_DIR}/third_party/ggpo/src/include
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2Mixer_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${Gperftools_INCLUDE_DIRS}
)

add_executable(kusoge ${SOURCE} ${HEADER})

target_link_libraries( 
    kusoge 
    PRIVATE nlohmann_json::nlohmann_json
    ${PROJECT_SOURCE_DIR}/third_party/ggpo/build/src/libGGPO.dylib
    ${Gperftools_LIBRARIES}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
    ${Boost_LIBRARIES}
    ${SDL2Mixer_LIBRARY}
    ${SDL2_TTF_LIBRARIES}
)
