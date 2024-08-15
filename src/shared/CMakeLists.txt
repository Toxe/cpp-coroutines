add_library(lib_shared
    fibonacci.cpp
    fibonacci.hpp
    print_and_assert_result.hpp
)

set_target_properties(lib_shared PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(lib_shared PUBLIC cxx_std_23)
target_compile_options(lib_shared PRIVATE ${SANITIZER_COMPILE_OPTIONS} ${DEFAULT_COMPILER_OPTIONS} ${DEFAULT_COMPILER_WARNINGS})
target_link_options(lib_shared PRIVATE ${SANITIZER_LINK_OPTIONS})
target_link_libraries(lib_shared PRIVATE ${SANITIZER_LINK_LIBRARIES})
target_include_directories(lib_shared INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
