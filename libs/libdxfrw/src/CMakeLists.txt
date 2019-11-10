file(GLOB src
    "*.cpp"
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
