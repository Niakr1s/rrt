cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-mwindows")

project(rrt LANGUAGES CXX VERSION 0.2.0)

set(BUILD_SHARED_LIBS ON)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/a")

configure_file(src/version.h.in ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/version.h)

add_definitions( -DBOOST_ALL_DYN_LINK )

if (COUT_LOG)
    add_definitions(-DCOUT_LOG)
endif()

if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
    set(Boost_ARCHITECTURE "-x64")
else()
    set(Boost_ARCHITECTURE "-x32")
endif()

find_package(Boost REQUIRED COMPONENTS
    log filesystem date_time
)

set(BUILD_TOOLS OFF)
set(BUILD_REGRESS OFF)
set(BUILD_EXAMPLES OFF)
set(BUILD_DOC OFF)
set(BUILD_TESTS OFF)

configure_file(libs/openssl/libcrypto-1_1-x64.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libcrypto-1_1-x64.dll COPYONLY)
configure_file(libs/openssl/libssl-1_1-x64.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libssl-1_1-x64.dll COPYONLY)

add_subdirectory(libs/pugixml)
add_subdirectory(libs/fmt)
add_subdirectory(libs/libdxfrw)
add_subdirectory(libs/libzip)

add_subdirectory(src)

if (ENABLE_TESTS)
add_subdirectory(tests)
endif()
