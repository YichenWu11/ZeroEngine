set(imgui_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking)

message(STATUS ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking)

add_library(imgui STATIC
    ${imgui_SOURCE_DIR_}/backends/imgui_impl_win32.cpp
    ${imgui_SOURCE_DIR_}/backends/imgui_impl_dx12.cpp
    ${imgui_SOURCE_DIR_}/imgui.cpp
    ${imgui_SOURCE_DIR_}/imgui_demo.cpp
    ${imgui_SOURCE_DIR_}/imgui_draw.cpp
    ${imgui_SOURCE_DIR_}/imgui_tables.cpp
    ${imgui_SOURCE_DIR_}/imgui_widgets.cpp
)

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR_})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR_}/backends)

# target_link_libraries(${TARGET_NAME} INTERFACE imgui)