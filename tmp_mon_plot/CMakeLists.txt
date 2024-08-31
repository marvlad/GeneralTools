cmake_minimum_required(VERSION 2.8.9)
project(mrd_mon)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(GCC_Flags_For_CXX "-Wall -fPIC -pthread -Wformat=0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_Flags_For_CXX}")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")

# Find ROOT package
find_package(ROOT REQUIRED COMPONENTS Hist)

# Include ROOT headers and project headers
include_directories(${ROOT_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

# Link ROOT libraries
link_directories(${ROOT_LIBRARY_DIRS})

# Add source files
set(SOURCES src/mrd_mon.cpp)

# Add executable
add_executable(mrd_mon ${SOURCES})

# Link libraries
target_link_libraries(mrd_mon ${ROOT_LIBRARIES})
