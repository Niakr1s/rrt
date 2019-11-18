set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(rrt
    main.cpp
)

target_link_libraries(rrt
    dxf
    xml
    Qt5::Widgets
)

target_include_directories(rrt PUBLIC
    ../
)
