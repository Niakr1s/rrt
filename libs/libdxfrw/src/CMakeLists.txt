find_package(Boost REQUIRED COMPONENTS
    filesystem
)

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
   Boost::filesystem
)

include_directories(spatial
    ${Boost_INCLUDE_DIRS}
)
