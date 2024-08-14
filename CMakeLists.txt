cmake_minimum_required(VERSION 3.22)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project("C++ Coroutines" LANGUAGES CXX)

# testing settings
include(cmake/Testing.cmake)

# adjust or override some CMake defaults
include(cmake/OverrideCMakeDefaults.cmake)

# default compiler options and warnings
include(cmake/DefaultCompilerOptions.cmake)
include(cmake/DefaultCompilerWarnings.cmake)

# static analyzers
include(cmake/StaticAnalyzers.cmake)

# sanitizers
include(cmake/Sanitizers.cmake)

find_package(nanobench CONFIG REQUIRED)
find_package(cppcoro CONFIG REQUIRED)

add_subdirectory(src/cppcoro)
add_subdirectory(src/manual)
add_subdirectory(src/functions)
