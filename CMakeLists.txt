cmake_minimum_required(VERSION 3.16)

project(calc LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(nlohmann_json)

add_executable(calc
    src/main.cpp
    src/runner.cpp
    src/parser.cpp
    src/checker.cpp
    src/calculator.cpp
    src/printer.cpp
)

set_target_properties(calc PROPERTIES
    CXX_CLANG_TIDY "clang-tidy"
)

target_link_libraries(calc PRIVATE nlohmann_json::nlohmann_json)

target_compile_features(calc PRIVATE cxx_std_20)

install(TARGETS calc
    RUNTIME DESTINATION bin
)

add_custom_target(calc_format
    COMMAND clang-format -i
        ${CMAKE_CURRENT_SOURCE_DIR}/include/runner.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/parser.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/checker.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/calculator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/printer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/runner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/checker.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calculator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/printer.cpp
    COMMENT "Running clang-format"
)