cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(rrt CXX)

if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    set(Boost_ARCHITECTURE "-x64")
else()
    set(Boost_ARCHITECTURE "-x32")
endif()

find_package(Boost REQUIRED COMPONENTS
    log filesystem
)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(libs/pugixml)
add_subdirectory(libs/fmt)
