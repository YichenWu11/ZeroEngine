set(TARGET_NAME SandBox)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS ./*.cpp)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS ./*.h)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

add_executable(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})

set_target_properties(${TARGET_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_definitions(${TARGET_NAME} PUBLIC ZE_ROOT_DIR=${CMAKE_SOURCE_DIR})

target_include_directories(${TARGET_NAME}
    PUBLIC ${ZE_ROOT_DIR}/zeroengine/source/sandbox/include
    PUBLIC ${ZE_ROOT_DIR}/zeroengine/source)

target_link_libraries(${TARGET_NAME} PUBLIC ZE::Runtime)
