# dxf

add_library(dxf SHARED
    dxf.h
    dxf.cpp
)

target_link_libraries(dxf
    Boost::log
    Boost::filesystem
    fmt::fmt
    dxfrw
    spatial
    cadastral_info
)

target_include_directories(dxf PUBLIC
    ../libs/libdxfrw/src
)

# xml

add_library(xml SHARED
    xml.h
    xml.cpp
    xmlspatial.h
    xmlspatial.cpp
    xmlparser.h
    xmlparser.cpp
    standardparser.h
    standardparser.cpp
)

target_include_directories(xml PUBLIC
    ${Boost_INCLUDE_DIRS}
    pugixml_INCLUDE_DIRS
)

target_link_libraries(xml
    fmt::fmt
    pugixml
    Boost::log
    Boost::filesystem
    spatial
    dxf
    cadastral_info
)

# spatial

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

include_directories(spatial
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(spatial
    fmt::fmt
    Boost::log
)

# cadastral_info

add_library(cadastral_info SHARED
    xmlspatialinfo.h
    xmlspatialinfo.cpp
    xmlinfo.h
    xmlinfo.cpp
    cadastralnumber.h
    cadastralnumber.cpp
)

include_directories(cadastral_info
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(cadastral_info
    fmt::fmt
    Boost::date_time
    Boost::filesystem
)


# rzip

add_library(rrtzip SHARED
    rrtzip.h
    rrtzip.cpp
)

include_directories(rrtzip
    ${Boost_INCLUDE_DIRS}
    ${libzip_SOURCE_DIR}/lib
)

target_link_libraries(rrtzip
    Boost::filesystem
    zip
)

add_subdirectory(gui)
