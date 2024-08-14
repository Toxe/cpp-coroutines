set(ALL_TARGETS
    uppercase
)

foreach(target ${ALL_TARGETS})
    add_executable(${target} ${target}.cpp)
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
    target_compile_features(${target} PUBLIC cxx_std_23)
    target_compile_options(${target} PRIVATE ${SANITIZER_COMPILE_OPTIONS} ${DEFAULT_COMPILER_OPTIONS} ${DEFAULT_COMPILER_WARNINGS})
    target_link_options(${target} PRIVATE ${SANITIZER_LINK_OPTIONS})
    target_link_libraries(${target} PRIVATE ${SANITIZER_LINK_LIBRARIES})

    set_target_properties(${target} PROPERTIES FOLDER "${target}")
    get_target_property(${target}_sources ${target} SOURCES)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Sources" FILES ${target}_sources)
endforeach()
