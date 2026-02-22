cmake_minimum_required(VERSION 3.16)

project(calc LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(nlohmann_json googletest spdlog)

add_library(calc_lib
    src/runner.cpp
    src/parser.cpp
    src/checker.cpp
    src/calculator.cpp
    src/printer.cpp
)

target_include_directories(calc_lib PUBLIC include)

target_link_libraries(calc_lib
    PUBLIC 
    nlohmann_json::nlohmann_json 
    spdlog::spdlog
)

add_executable(calc src/main.cpp)

target_link_libraries(calc PRIVATE calc_lib)

enable_testing()

add_executable(calc_tests
    tests/runner_tests.cpp
)

target_link_libraries(calc_tests
    PRIVATE
        calc_lib
        gtest_main
)

add_test(NAME calc_tests COMMAND calc_tests)


set_target_properties(calc PROPERTIES
    CXX_CLANG_TIDY "clang-tidy"
)

target_compile_features(calc_lib PUBLIC cxx_std_20)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/include/logger.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/runner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/checker.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calculator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/printer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}tests/runner_tests.cpp
    COMMENT "Running clang-format"
)