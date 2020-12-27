project(kusoge)

cmake_minimum_required(VERSION 3.9)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-no_pie,-export_dynamic")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON) # -std=c++11 instead of -std=gnu++11
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
SET(CMAKE_INSTALL_RPATH "${PROJECT_SOURCE_DIR}/third_party")
set(MY_CXX_COMPILE_FEATURES cxx_generic_lambdas cxx_range_for cxx_strong_enums)
set(JSON_BuildTests OFF CACHE INTERNAL "")

file(GLOB_RECURSE SOURCE src/*.cpp)
file(GLOB_RECURSE HEADER include/*.h)

find_package(nlohmann_json REQUIRED)
find_package(PkgConfig REQUIRED)
# find_package(SDL2 REQUIRED)
# find_package(SDL2_image REQUIRED)
# find_package(SDL2Mixer REQUIRED)
# find_package(SDL2_ttf REQUIRED)
find_package(Boost 1.55 REQUIRED system serialization)
# find_package(Gperftools REQUIRED COMPONENTS TCMALLOC_AND_PROFILER)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/third_party/ggpo/src/include
    ${PROJECT_SOURCE_DIR}/third_party/SDL2.Framework/Headers
    ${PROJECT_SOURCE_DIR}/third_party/SDL2_mixer.Framework/Headers
    ${PROJECT_SOURCE_DIR}/third_party/SDL2_image.Framework/Headers
    ${PROJECT_SOURCE_DIR}/third_party/SDL2_ttf.Framework/Headers
    ${Boost_INCLUDE_DIRS}
    # ${Gperftools_INCLUDE_DIRS}
)

add_executable(kusoge ${SOURCE} ${HEADER})

target_link_libraries( 
    kusoge 
    PRIVATE nlohmann_json::nlohmann_json
    ${PROJECT_SOURCE_DIR}/third_party/ggpo/build/src/libGGPO.a
    ${PROJECT_SOURCE_DIR}/third_party/SDL2.Framework/SDL2
    ${PROJECT_SOURCE_DIR}/third_party/SDL2_mixer.Framework/SDL2_mixer
    ${PROJECT_SOURCE_DIR}/third_party/SDL2_image.Framework/SDL2_image
    ${PROJECT_SOURCE_DIR}/third_party/SDL2_ttf.Framework/SDL2_ttf
    ${Boost_LIBRARIES}
    # ${Gperftools_LIBRARIES}
)
