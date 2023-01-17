set(TARGET_NAME ZE_Editor)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS source/*.cpp include/*.h)

# add_library(${TARGET_NAME} SHARED ${srcs})
add_executable(${TARGET_NAME} ${srcs})

set_target_properties(${TARGET_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_definitions(${TARGET_NAME} PUBLIC ZE_ROOT_DIR=${CMAKE_SOURCE_DIR})

target_include_directories(${TARGET_NAME}
    PUBLIC ${ZE_ROOT_DIR}/zeroengine/source/editor/include
    PUBLIC ${ZE_ROOT_DIR}/zeroengine/source)

target_link_libraries(${TARGET_NAME} PUBLIC ZE::Runtime)
