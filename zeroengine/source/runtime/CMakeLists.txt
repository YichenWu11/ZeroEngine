set(TARGET_NAME ZE_Runtime)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS ./*.cpp)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS ./*.h)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

add_library(${TARGET_NAME} ${SOURCE_FILES} ${HEADER_FILES})
add_library(ZE::Runtime ALIAS ${TARGET_NAME})

target_include_directories(${TARGET_NAME} PUBLIC ${ZE_ROOT_DIR}/zeroengine/source)

target_precompile_headers(${TARGET_NAME} PUBLIC ${ZE_ROOT_DIR}/zeroengine/source/runtime/pch.h)

target_compile_definitions(${TARGET_NAME} PUBLIC ZE_ROOT_DIR=${CMAKE_SOURCE_DIR})

target_link_libraries(${TARGET_NAME} PUBLIC Chen::CDX12)
target_link_libraries(${TARGET_NAME} PUBLIC crossguid)
target_link_libraries(${TARGET_NAME} PUBLIC spdlog::spdlog)
target_link_libraries(${TARGET_NAME} PUBLIC rapidjson)
target_link_libraries(${TARGET_NAME} PUBLIC imgui)
target_link_libraries(${TARGET_NAME} PUBLIC entt)
target_link_libraries(${TARGET_NAME} PUBLIC ImGuizmo)
target_link_libraries(${TARGET_NAME} PUBLIC box2d)