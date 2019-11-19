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
    ../libs/libdxfrw/include
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
    idb.h
    idb.cpp
    db.h
    db.cpp
    sqldb.h
    sqldb.cpp
)

target_include_directories(xml PUBLIC
    ${Boost_INCLUDE_DIRS}
    pugixml_INCLUDE_DIRS
    ../libs/sqlite
)

target_link_libraries(xml
    fmt::fmt
    pugixml
    Boost::log
    Boost::filesystem
    spatial
    dxf
    cadastral_info
    sqlite
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
)

add_subdirectory(gui)
