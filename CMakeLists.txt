cmake_minimum_required(VERSION 3.13)
project(stdman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(Boost REQUIRED)
find_package(glog CONFIG REQUIRED)
find_package(Threads)

link_libraries(glog::glog ${CMAKE_THREAD_LIBS_INIT})

include_directories(src)
add_executable(stdman src/main.cpp)
