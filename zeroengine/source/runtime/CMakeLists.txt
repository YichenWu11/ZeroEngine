set(TARGET_NAME ZE_Runtime)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS ./*.cpp ./*.h)

add_library(${TARGET_NAME} ${srcs})
add_library(ZE::Runtime ALIAS ${TARGET_NAME})

target_include_directories(${TARGET_NAME} PUBLIC ${ZE_ROOT_DIR}/zeroengine/source)

target_link_libraries(${TARGET_NAME} PUBLIC Chen::CDX12)
target_link_libraries(${TARGET_NAME} PUBLIC crossguid)
target_link_libraries(${TARGET_NAME} PUBLIC spdlog::spdlog)
target_link_libraries(${TARGET_NAME} PUBLIC rapidjson)
