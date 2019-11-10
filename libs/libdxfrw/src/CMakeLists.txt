file(GLOB src
    "*.cpp"
    "*.h"
    "intern/*.cpp"
    "intern/*.h"
)

add_library(dxfrw SHARED ${src})

target_link_libraries(dxfrw
    iconv
)

target_link_options(dxfrw PUBLIC -liconv)
