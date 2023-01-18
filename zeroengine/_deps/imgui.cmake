set(imgui_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

file(GLOB imgui_sources CONFIGURE_DEPENDS "${imgui_SOURCE_DIR_}/*.cpp")
file(GLOB imgui_impl CONFIGURE_DEPENDS
    "${imgui_SOURCE_DIR_}/backends/imgui_impl_win32.cpp"
    "${imgui_SOURCE_DIR_}/backends/imgui_impl_win32.h"
    "${imgui_SOURCE_DIR_}/backends/imgui_impl_dx12.cpp"
    "${imgui_SOURCE_DIR_}/backends/imgui_impl_dx12.h")

add_library(imgui STATIC ${imgui_sources} ${imgui_impl})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR_})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR_}/backends)