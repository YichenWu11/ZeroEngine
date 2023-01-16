set(crossguid_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/crossguid)

add_library(crossguid STATIC
    ${crossguid_SOURCE_DIR_}/src/guid.cpp
    ${crossguid_SOURCE_DIR_}/include/crossguid/guid.hpp)

target_include_directories(crossguid PUBLIC ${crossguid_SOURCE_DIR_}/include)
