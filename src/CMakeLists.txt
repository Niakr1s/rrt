set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

add_executable(rrt
    main.cpp
)

target_link_libraries(rrt
    rrt_lib
)


add_library(rrt_lib
    xml.cpp
    xmlcadastralnumber.cpp
    xmlcadastralnumberinfo.cpp
    xmlparser.cpp
    standardparser.cpp
    xmlpoint.cpp
)

include_directories(rrt_lib
    ${Boost_INCLUDE_DIRS}
    pugixml_INCLUDE_DIRS
)

target_link_libraries(rrt_lib
    fmt::fmt
    pugixml
    Qt5::Widgets
    Qt5::Core
    Boost::log
    Boost::filesystem
)
