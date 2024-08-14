set(ALL_TARGETS
    tasks
)

foreach(target ${ALL_TARGETS})
    add_executable(functions_${target} ${target}.cpp)
    set_target_properties(functions_${target} PROPERTIES CXX_EXTENSIONS OFF OUTPUT_NAME ${target})
    target_compile_features(functions_${target} PUBLIC cxx_std_23)
    target_compile_options(functions_${target} PRIVATE ${SANITIZER_COMPILE_OPTIONS} ${DEFAULT_COMPILER_OPTIONS} ${DEFAULT_COMPILER_WARNINGS})
    target_link_options(functions_${target} PRIVATE ${SANITIZER_LINK_OPTIONS})
    target_link_libraries(functions_${target} PRIVATE ${SANITIZER_LINK_LIBRARIES} nanobench::nanobench)
endforeach()
