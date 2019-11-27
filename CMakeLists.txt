# only for cmake --version >= 3.5.1
cmake_minimum_required(VERSION 3.5.1)

# project name
project(informatica_grafica)

# creates the variable EXEC and sets it to test
set(RAY_TRACER Ray_tracer)
set(TONE_MAPPING Tone_mapping)
set(PATH_TRACER Path_tracer)

# set the C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -O3")

# I../includes
include_directories(includes)

# puts all .cpp files inside src to the SOURCES variable
file(GLOB RAY_TRACING src/*.cpp)
file(GLOB PATH_TRACING src/*.cpp)
get_filename_component(TM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/Tone_mapping.cpp ABSOLUTE)
get_filename_component(PT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/Path_tracer.cpp ABSOLUTE)
get_filename_component(RT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/Ray_tracer.cpp ABSOLUTE)
list(REMOVE_ITEM RAY_TRACING ${TM_SRC} ${PT_SRC})
list(REMOVE_ITEM PATH_TRACING ${TM_SRC} ${RT_SRC})
set(TMAPPING ${TM_SRC} src/RGB.cpp)


# compiles the files defined by SOURCES to generante the executable defined by EXEC
add_executable(${RAY_TRACER} ${RAY_TRACING})
add_executable(${TONE_MAPPING} ${TMAPPING})
add_executable(${PATH_TRACER} ${PATH_TRACING})