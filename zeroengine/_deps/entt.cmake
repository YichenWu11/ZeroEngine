set(entt_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/entt)

add_library(entt INTERFACE)

target_include_directories(entt INTERFACE ${entt_SOURCE_DIR_}/include)
