project(ZE_RunTime)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS ./*.cpp ./*.h)

add_library(${PROJECT_NAME} ${srcs})

target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)