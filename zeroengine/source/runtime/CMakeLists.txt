project(ZE_Runtime)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS ./*.cpp ./*.h)

add_library(${PROJECT_NAME} ${srcs})
add_library(ZE::Runtime ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${ZE_ROOT_DIR}/zeroengine/source)

target_link_libraries(${PROJECT_NAME} PUBLIC Chen::CDX12)