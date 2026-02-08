cmake_minimum_required(VERSION 3.16)

project(calc LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(operations_lib
    GIT_REPOSITORY git@github.com:Razred/Operations_Library.git
    GIT_TAG v1.0.1
)

FetchContent_MakeAvailable(operations_lib)

add_executable(calc
    src/calc_core.cpp
    src/main.cpp
)

set_target_properties(calc PROPERTIES
    CXX_CLANG_TIDY "clang-tidy"
)

target_link_libraries(calc PRIVATE operations_lib)

target_compile_features(calc PRIVATE cxx_std_17)

install(TARGETS calc
    RUNTIME DESTINATION bin
)

add_custom_target(calc_format
    COMMAND clang-format -i
        ${CMAKE_CURRENT_SOURCE_DIR}/include/calc_core.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calc_core.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running clang-format"
)