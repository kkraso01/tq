cmake_minimum_required(VERSION 3.20)
project(tq LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build tq-core first
add_subdirectory(tq-core)

# Build CLI
add_subdirectory(tq-cli)

# (optional next)
# add_subdirectory(pytq)
