project(type_utils)

set(HEADERS
    type_list.h
    type_tree.h
    type_set.h
    type_info.h
    type_universal_set.h
    type_map.h
)

set(SOURCES
)

add_library(type_utils ${HEADERS} ${SOURCES})

target_include_directories(type_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(type_utils PRIVATE cxx_std_20)

set_target_properties(type_utils PROPERTIES LINKER_LANGUAGE CXX)