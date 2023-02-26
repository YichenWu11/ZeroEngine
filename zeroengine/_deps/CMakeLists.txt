add_subdirectory(CDX12-Backend)

if(NOT TARGET crossguid)
    include(crossguid.cmake)
endif()

if(NOT TARGET rapidjson)
    include(rapidjson.cmake)
endif()

if(NOT TARGET spdlog)
    option(SPDLOG_BUILD_EXAMPLE "" OFF)
    option(SPDLOG_INSTALL "" OFF)
    add_subdirectory(spdlog)
endif()

if(NOT TARGET imgui)
    include(imgui.cmake)
endif()

if(NOT TARGET entt)
    include(entt.cmake)
endif()

if(NOT TARGET ImGuizmo)
    include(ImGuizmo.cmake)
endif()

if(NOT TARGET box2d)
    add_subdirectory(box2d)
endif()
