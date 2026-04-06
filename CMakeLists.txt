cmake_minimum_required(VERSION 3.16)

project(calc LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "Building calc")

option(USE_THREAD_SANITIZER "Use ThreadSanitizer" OFF)

if(USE_THREAD_SANITIZER)
    message(STATUS "ThreadSanitizer is ENABLED")

    add_compile_options(
        -fsanitize=thread
        -fno-omit-frame-pointer
        -g
        -O1
    )

    add_link_options(
        -fsanitize=thread
    )
endif()

# VALGRIND
find_program(VALGRIND valgrind)
if(VALGRIND)
    set(MEMORYCHECK_COMMAND ${VALGRIND})
    set(MEMORYCHECK_COMMAND_OPTIONS "--leak-check=full --show-leak-kinds=all --error-exitcode=1")
endif()

include(FetchContent)

# Отключаем install у зависимостей (для CPack) 
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(SPDLOG_INSTALL OFF CACHE BOOL "" FORCE)
set(JSON_Install OFF CACHE BOOL "" FORCE)
set(CPPZMQ_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CPPZMQ_INSTALL OFF CACHE BOOL "" FORCE)


# Подключение библиотек
FetchContent_Declare(operations_lib
    GIT_REPOSITORY https://github.com/Razred/Operations_Library.git
    GIT_TAG v1.1.2
)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)

FetchContent_Declare(
  cppzmq
  GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
  GIT_TAG v4.11.0
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

FetchContent_MakeAvailable(
    operations_lib 
    nlohmann_json 
    cppzmq 
    googletest 
    spdlog
)

# PostgreSQL
find_package(PostgreSQL REQUIRED)

# Основной бинарник
add_library(calc_lib
    src/runner.cpp
    src/parser.cpp
    src/checker.cpp
    src/calculator.cpp
    src/printer.cpp
    src/logger.cpp
    src/postgres_db.cpp

    SignalHandler/signal_handler.cpp
    ZmqServer/zmq_server.cpp
    SupervisorRunner/supervisor_runner.cpp
)

target_include_directories(calc_lib PUBLIC include)

target_compile_features(calc_lib PUBLIC cxx_std_20)

target_link_libraries(calc_lib
    PUBLIC
        operations_lib
        nlohmann_json::nlohmann_json
        cppzmq
        spdlog::spdlog
    PRIVATE
        PostgreSQL::PostgreSQL
)

add_executable(calc src/main.cpp)

target_link_libraries(calc PRIVATE calc_lib)


# Тесты
include(CTest)
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


#Клиент
add_executable(client client/main.cpp)

target_link_libraries(client
    PRIVATE
        nlohmann_json::nlohmann_json
        cppzmq
)


# DEB
include(GNUInstallDirs)

install(TARGETS calc
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT calc_runtime
)

install(FILES systemd/calc.service
    DESTINATION /lib/systemd/system
    COMPONENT calc_runtime
)


# Cpack
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "calc")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Calculator Linux service")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Razred")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_RELEASE "1")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

# Пакуем только наш компонент, а не Unspecified от зависимостей
set(CPACK_COMPONENTS_ALL calc_runtime)
set(CPACK_DEB_COMPONENT_INSTALL ON)

include(CPack)


set_target_properties(calc PROPERTIES
    CXX_CLANG_TIDY "clang-tidy"
)

add_custom_target(calc_format
    COMMAND clang-format -i
        ${CMAKE_CURRENT_SOURCE_DIR}/include/runner.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/parser.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/checker.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/calculator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/printer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/logger.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/config.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/signal_handler.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/zmq_server.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/supervisor_runner.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/runner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/checker.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calculator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/printer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/logger.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SignalHandler/signal_handler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ZmqServer/zmq_server.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SupervisorRunner/supervisor_runner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/runner_tests.cpp
    COMMENT "Running clang-format"
)