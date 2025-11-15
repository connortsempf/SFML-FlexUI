#### Examples Configuration ####
## Compile Source Examples Source Files ##
file (GLOB EXAMPLE_SOURCES "*.cpp")
## Create Executables for Example Source Files ##
foreach(EX ${EXAMPLE_SOURCES})
    get_filename_component(EX_NAME ${EX} NAME_WE)
    add_executable(${EX_NAME} ${EX})
    target_link_libraries(${EX_NAME} PRIVATE sfml-flexui)
    set_target_properties(${EX_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/examples"
    )
endforeach()
