set(TARGET_NAME Runtime)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS ./*.cpp)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS ./*.h)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

add_library(${TARGET_NAME} ${SOURCE_FILES} ${HEADER_FILES})
add_library(ZE::Runtime ALIAS ${TARGET_NAME})

target_include_directories(${TARGET_NAME} PUBLIC ${ROOT_DIR}/zeroengine/source)

target_precompile_headers(${TARGET_NAME} PUBLIC ${ROOT_DIR}/zeroengine/source/runtime/pch.h)

target_compile_definitions(${TARGET_NAME} PUBLIC ROOT_DIR=${ROOT_DIR})

target_compile_options(${TARGET_NAME} PRIVATE /bigobj)

find_package(RapidJSON CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(Lua REQUIRED)

target_include_directories(${TARGET_NAME} PUBLIC ${LUA_INCLUDE_DIR})

target_link_libraries(${TARGET_NAME} PUBLIC ${LUA_LIBRARIES})
target_link_libraries(${TARGET_NAME} PUBLIC sol2)
target_link_libraries(${TARGET_NAME} PUBLIC Chen::CDX12)
target_link_libraries(${TARGET_NAME} PUBLIC spdlog::spdlog)
target_link_libraries(${TARGET_NAME} PUBLIC rapidjson)
target_link_libraries(${TARGET_NAME} PUBLIC EnTT::EnTT)
target_link_libraries(${TARGET_NAME} PUBLIC box2d::box2d)
target_link_libraries(${TARGET_NAME} PUBLIC imgui)
target_link_libraries(${TARGET_NAME} PUBLIC ImGuizmo)
