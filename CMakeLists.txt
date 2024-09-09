cmake_minimum_required(VERSION 3.24)
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

include(FetchContent)
FetchContent_Declare(cppcoro GIT_REPOSITORY https://github.com/andreasbuhr/cppcoro.git GIT_TAG main)
FetchContent_MakeAvailable(cppcoro)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libcoro REQUIRED IMPORTED_TARGET libcoro)

find_package(nanobench CONFIG REQUIRED)
find_package(concurrencpp CONFIG REQUIRED)

add_subdirectory(src/shared)
add_subdirectory(src/reference)
add_subdirectory(src/manual)
add_subdirectory(src/concurrencpp)
add_subdirectory(src/cppcoro)
add_subdirectory(src/libcoro)
add_subdirectory(src/std)
