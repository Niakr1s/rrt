file(GLOB src
    "*.h"
    "*.cpp"
    "intern/*.h"
    "intern/*.cpp"
)

add_library(dxfrw SHARED
    ${src}
)

target_link_libraries(dxfrw
    iconv
)

target_link_options(dxfrw PUBLIC
    -liconv
)
