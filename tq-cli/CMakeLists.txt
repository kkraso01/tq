cmake_minimum_required(VERSION 3.20)
project(tq-cli VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(tq main.cpp)

# Link against tq_core_static built by root CMakeLists
target_link_libraries(tq PRIVATE tq_core_static)
target_include_directories(tq PRIVATE ${CMAKE_SOURCE_DIR}/tq-core/include)

install(TARGETS tq RUNTIME DESTINATION bin)
