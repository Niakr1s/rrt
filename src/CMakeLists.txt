set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

add_executable(rrt
    main.cpp
)

add_library(dxf SHARED
    dxf.h
    dxf.cpp
)

add_library(xml SHARED
    xml.h
    xml.cpp
    xmlspatial.h
    xmlspatial.cpp
    xmlspatialinfo.h
    xmlspatialinfo.cpp
    xmlparser.h
    xmlparser.cpp
    standardparser.h
    standardparser.cpp
    xmlinfo.h
    xmlinfo.cpp
    cadastralnumber.h
    cadastralnumber.cpp
)

add_library(spatial SHARED
    point.h
    point.cpp
    spatial.h
    spatial.cpp
    rect.h
    rect.cpp
    compare.h
    compare.cpp
)

target_include_directories(dxf PUBLIC
    ../libs/libdxfrw/include
)

target_link_libraries(dxf
    Boost::log
    Boost::filesystem
    fmt::fmt
    dxfrw
    spatial
)

include_directories(xml
    ${Boost_INCLUDE_DIRS}
    pugixml_INCLUDE_DIRS
)

target_link_libraries(xml
    fmt::fmt
    pugixml
    Boost::log
    Boost::filesystem
    spatial
)

include_directories(spatial
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(spatial
    fmt::fmt
    Boost::log
)

cotire(xml)
cotire(spatial)
cotire(rrt)
