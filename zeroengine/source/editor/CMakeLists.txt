project(ZE_Editor)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS source/*.cpp include/*.h)

add_executable(${PROJECT_NAME} ${srcs})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_compile_definitions(${PROJECT_NAME} PUBLIC ZE_ROOT_DIR=${CMAKE_SOURCE_DIR})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${ZE_ROOT_DIR}/zeroengine/source)

target_link_libraries(${PROJECT_NAME} PUBLIC ZE::Runtime)
