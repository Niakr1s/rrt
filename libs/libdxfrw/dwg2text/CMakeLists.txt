file(GLOB src
    "*.cpp"
    "*.h"
)

add_executable(dwg2text ${src})

target_link_libraries(dwg2text 
    dxfrw
)

include_directories(dwg2dxf ../include)
