set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

add_executable(rrt
    main.cpp
)

add_library(dxf SHARED
    dxf.cpp
)

add_library(xml SHARED
    xml.cpp
    xmlspatial.cpp
    xmlspatialinfo.cpp
    xmlparser.cpp
    standardparser.cpp
)

add_library(spatial SHARED
    point.cpp
    spatial.cpp
    rect.cpp
    compare.cpp
)

include_directories(dxf
    ../libs/libdxfrw/include
)

target_link_libraries(dxf
    dxfrw
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
