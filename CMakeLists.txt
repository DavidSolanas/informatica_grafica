# only for cmake --version >= 3.5.1
cmake_minimum_required(VERSION 3.5.1)

# project name
project(informatica_grafica)

# creates the variable EXEC and sets it to test
set(RAY_TRACER Ray_tracer)
set(TONE_MAPPING Tone_mapping)

# set the C++17 standard
set(CMAKE_CXX_STANDARD 17)

# I../includes
include_directories(includes)

# puts all .cpp files inside src to the SOURCES variable
file(GLOB SOURCES src/*.cpp)
get_filename_component(TM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/Tone_mapping.cpp ABSOLUTE)
list(REMOVE_ITEM SOURCES ${TM_SRC})
set(TMAPPING ${TM_SRC} src/RGB.cpp)


# compiles the files defined by SOURCES to generante the executable defined by EXEC
add_executable(${RAY_TRACER} ${SOURCES})
add_executable(${TONE_MAPPING} ${TMAPPING})