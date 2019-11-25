set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Widgets LinguistTools Network REQUIRED)

set(SRC
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
    xmltreebuttons.h
    xmltreebuttons.cpp
    xmltreesortfilterproxymodel.h
    xmltreesortfilterproxymodel.cpp
    updater.h
    updater.cpp
    xmlroottreeitem.h
    xmlroottreeitem.cpp
    util.h
    util.cpp
    typedefs.h
)

file(GLOB TS_FILES
    ts/*.ts
)

set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/i10n)

if (CREATE_TRANSLATIONS)
    qt5_create_translation(QM_FILES ${SRC} ${TS_FILES})
else()
    qt5_add_translation(QM_FILES ${TS_FILES})
endif()

add_executable(rrt
    ${SRC}
    ${QM_FILES}
    rc/rrt.o
)

target_link_libraries(rrt
    dxf
    xml
    Qt5::Widgets
    Boost::log
    Qt5::Network
)

target_include_directories(rrt PUBLIC
    ../
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
)
