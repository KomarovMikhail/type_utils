cmake_minimum_required(VERSION 3.22)
project(type_utils_example)

add_subdirectory(type_utils)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    main.cpp
)

add_executable(type_utils_example ${SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC type_utils)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/type_utils)

target_compile_features(type_utils_example PRIVATE cxx_std_20)