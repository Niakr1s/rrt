cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(rrt CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(BUILD_SHARED_LIBS TRUE)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    set(Boost_ARCHITECTURE "-x64")
else()
    set(Boost_ARCHITECTURE "-x32")
endif()

find_package(Boost REQUIRED COMPONENTS
    log filesystem
)

add_subdirectory(libs/pugixml)

add_subdirectory(libs/fmt)

add_subdirectory(src)
add_subdirectory(tests)
