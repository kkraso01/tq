cmake_minimum_required(VERSION 3.20)
project(tq-core VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Library sources
set(TQ_SOURCES
    src/value.cpp
    src/lexer.cpp
    src/parser.cpp
    src/evaluator.cpp
    src/toon_parser.cpp
    src/tq.cpp
)

set(TQ_HEADERS
    include/tq/value.hpp
    include/tq/lexer.hpp
    include/tq/parser.hpp
    include/tq/evaluator.hpp
    include/tq/toon_parser.hpp
    include/tq/tq.hpp
    include/tq/ast.hpp
)

# Core library
add_library(tq_core SHARED ${TQ_SOURCES} ${TQ_HEADERS})
target_include_directories(tq_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Static library variant
add_library(tq_core_static STATIC ${TQ_SOURCES} ${TQ_HEADERS})
target_include_directories(tq_core_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Tests
enable_testing()
add_subdirectory(tests)

# Installation
install(TARGETS tq_core tq_core_static
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/tq DESTINATION include)
