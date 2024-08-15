set(ALL_TARGETS
    tasks
)

foreach(target ${ALL_TARGETS})
    add_executable(cppcoro_${target} ${target}.cpp)
    set_target_properties(cppcoro_${target} PROPERTIES CXX_EXTENSIONS OFF OUTPUT_NAME ${target})
    target_compile_features(cppcoro_${target} PUBLIC cxx_std_23)
    target_compile_options(cppcoro_${target} PRIVATE ${SANITIZER_COMPILE_OPTIONS} ${DEFAULT_COMPILER_OPTIONS} ${DEFAULT_COMPILER_WARNINGS})
    target_link_options(cppcoro_${target} PRIVATE ${SANITIZER_LINK_OPTIONS})
    target_link_libraries(cppcoro_${target} PRIVATE ${SANITIZER_LINK_LIBRARIES} lib_shared nanobench::nanobench cppcoro)
endforeach()
