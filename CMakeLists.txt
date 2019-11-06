# only for cmake --version >= 3.5.1
cmake_minimum_required(VERSION 3.5.1)

# project name
project(informatica_grafica)

# creates the variable EXEC and sets it to test
set(EXEC test)

# set the C++17 standard
set(CMAKE_CXX_STANDARD 17)

# I../includes
include_directories(includes)

# puts all .cpp files inside src to the SOURCES variable
file(GLOB SOURCES src/*.cpp)

# compiles the files defined by SOURCES to generante the executable defined by EXEC
add_executable(${EXEC} ${SOURCES})

if(MSVC) # MSVC compiler (Win32 only)
    # Display more warnings
    set(CMAKE_CXX_FLAGS "/W3")
elseif(UNIX OR CMAKE_COMPILER_IS_GNUCXX) # Clang OR Gcc (Linux, Mac OS or Win32 with MingW)
    # Enable C++11 and displays all warnings
    set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
endif()