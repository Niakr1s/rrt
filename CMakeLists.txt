cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(rrt CXX)

find_package(Boost REQUIRED COMPONENTS
    log filesystem
)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(libs/pugixml)
add_subdirectory(libs/fmt)
