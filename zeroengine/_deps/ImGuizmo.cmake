set(ImGuizmo_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo)

message(STATUS ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo)

add_library(ImGuizmo STATIC
    ${ImGuizmo_SOURCE_DIR_}/GraphEditor.cpp
    ${ImGuizmo_SOURCE_DIR_}/ImCurveEdit.cpp
    ${ImGuizmo_SOURCE_DIR_}/ImGradient.cpp
    ${ImGuizmo_SOURCE_DIR_}/ImGuizmo.cpp
    ${ImGuizmo_SOURCE_DIR_}/ImSequencer.cpp
)

target_include_directories(ImGuizmo PUBLIC ${ImGuizmo_SOURCE_DIR_})

target_link_libraries(ImGuizmo PUBLIC imgui)