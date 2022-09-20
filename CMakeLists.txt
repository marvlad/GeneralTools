cmake_minimum_required(VERSION 2.8.9)
project (example)

set( GCC_Flags_For_CXX "-Wall -fPIC -pthread -Wformat=0 --std=c++0x" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_Flags_For_CXX}" )
set( CMAKE_CXX_FLAGS_DEBUG "-ggdb" )

find_package(ROOT)

add_executable(example example.cpp)
include_directories(/MAT/MAT/opt/include/)

target_link_libraries(example /MAT/MAT/opt/lib/libMAT.so ${ROOT_LIBRARIES})
