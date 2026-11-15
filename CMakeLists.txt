cmake_minimum_required(VERSION 3.20)

project(ObligatorioP4UDE LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Enable GCC coverage instrumentation" OFF)

add_library(p4_core
    src/Autor.cpp
    src/ColeccionAutores.cpp
    src/ColeccionLibros.cpp
    src/Escolar.cpp
    src/Fecha.cpp
    src/Fachada.cpp
    src/Libro.cpp
    src/Novela.cpp
    src/Texto.cpp
)
target_include_directories(p4_core PUBLIC include)

if(ENABLE_COVERAGE)
    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message(FATAL_ERROR "ENABLE_COVERAGE currently requires GCC (used by CI).")
    endif()
    target_compile_options(p4_core PRIVATE --coverage -O0 -g)
    target_link_options(p4_core PRIVATE --coverage)
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(p4_tests tests/main.cpp)
    target_link_libraries(p4_tests PRIVATE p4_core)
    if(ENABLE_COVERAGE)
        target_compile_options(p4_tests PRIVATE --coverage -O0 -g)
        target_link_options(p4_tests PRIVATE --coverage)
    endif()
    add_test(NAME p4_tests COMMAND p4_tests)
endif()
