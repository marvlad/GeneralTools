cmake_minimum_required(VERSION 2.8.9)
project (example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ROOT_DIR /minerva-MAT/root_src/install)

find_package(ROOT REQUIRED)

add_executable(example example.cpp)
include_directories(/minerva-MAT/opt/include/)

include_directories(${ROOT_INCLUDE_DIRS})

target_link_libraries(example /minerva-MAT/opt/lib/libMAT.so ${ROOT_LIBRARIES})
