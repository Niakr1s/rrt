file(GLOB src
    "*.cpp"
    "*.h"
)

add_executable(dwg2dxf ${src})

target_link_libraries(dwg2dxf
    dxfrw
)

include_directories(dwg2dxf ../include)
