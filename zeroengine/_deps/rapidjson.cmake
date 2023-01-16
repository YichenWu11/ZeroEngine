set(rapidjson_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson)

add_library(rapidjson INTERFACE)

target_include_directories(rapidjson INTERFACE ${rapidjson_SOURCE_DIR_}/include)
