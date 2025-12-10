cmake_minimum_required(VERSION 3.20)
project(pytq)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python
find_package(Python COMPONENTS Interpreter Development REQUIRED)

# Find pybind11
find_package(pybind11 CONFIG)
if(NOT pybind11_FOUND)
    # Try to find via pip install location
    execute_process(
        COMMAND ${Python_EXECUTABLE} -c "import pybind11; print(pybind11.get_cmake_dir())"
        OUTPUT_VARIABLE pybind11_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    find_package(pybind11 CONFIG PATHS ${pybind11_DIR})
endif()

if(NOT pybind11_FOUND)
    message(FATAL_ERROR "pybind11 not found. Install with: pip install pybind11")
endif()

# Link to tq-core
set(TQ_CORE_DIR ${CMAKE_SOURCE_DIR}/../tq-core)
set(TQ_CORE_BUILD ${TQ_CORE_DIR}/build)

if(WIN32)
    set(TQ_CORE_LIB ${TQ_CORE_BUILD}/Release/tq_core_static.lib)
    if(NOT EXISTS ${TQ_CORE_LIB})
        set(TQ_CORE_LIB ${TQ_CORE_BUILD}/tq_core_static.lib)
    endif()
else()
    set(TQ_CORE_LIB ${TQ_CORE_BUILD}/libtq_core_static.a)
endif()

# Python module
pybind11_add_module(pytq pytq.cpp)
target_include_directories(pytq PRIVATE ${TQ_CORE_DIR}/include)
target_link_libraries(pytq PRIVATE ${TQ_CORE_LIB})

# Set output name
set_target_properties(pytq PROPERTIES OUTPUT_NAME "pytq")
