set(ALL_TARGETS
    generator
    tasks
    uppercase
)

foreach(target ${ALL_TARGETS})
    add_executable(manual_${target} ${target}.cpp)
    set_target_properties(manual_${target} PROPERTIES CXX_EXTENSIONS OFF OUTPUT_NAME ${target})
    target_compile_features(manual_${target} PUBLIC cxx_std_23)
    target_compile_options(manual_${target} PRIVATE ${SANITIZER_COMPILE_OPTIONS} ${DEFAULT_COMPILER_OPTIONS} ${DEFAULT_COMPILER_WARNINGS})
    target_link_options(manual_${target} PRIVATE ${SANITIZER_LINK_OPTIONS})
    target_link_libraries(manual_${target} PRIVATE ${SANITIZER_LINK_LIBRARIES} lib_shared nanobench::nanobench)
endforeach()
