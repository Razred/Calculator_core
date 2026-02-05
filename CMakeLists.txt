cmake_minimum_required(VERSION 3.16)

project(calc LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(operations_lib
    GIT_REPOSITORY git@github.com:Razred/Operations_Library.git
    GIT_TAG v1.0.0
)

FetchContent_MakeAvailable(operations_lib)

add_executable(calc
    src/main.cpp
)

target_link_libraries(calc PRIVATE operations_lib)

target_compile_features(calc PRIVATE cxx_std_17)

install(TARGETS calc
    RUNTIME DESTINATION bin
)