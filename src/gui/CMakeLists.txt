set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(rrt
    main.cpp
    mainwindow.h
    mainwindow.cpp
    resources/resources.qrc
    mainwidget.h
    mainwidget.cpp
    xmltreemodel.h
    xmltreemodel.cpp
    xmltreeitem.h
    xmltreeitem.cpp
    dxflabel.h
    dxflabel.cpp
    xmltreedelegate.h
    xmltreedelegate.cpp
    xmltreeview.h
    xmltreeview.cpp
)

target_link_libraries(rrt
    dxf
    xml
    Qt5::Widgets
    Boost::log
)

target_include_directories(rrt PUBLIC
    ../
    ${Boost_INCLUDE_DIRS}
)
