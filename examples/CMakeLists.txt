#############################
#### Build Configuration ####
#############################

## Compile Examples Source Files ##
file(GLOB EXAMPLE_SOURCES src/*.cpp)

## Create the Executable ##
add_executable(Demo ${EXAMPLE_SOURCES})

## Obtain Examples Header Files ##
target_include_directories(Demo PUBLIC include)

## Link Against Built SFML-FlexUI ##
target_link_libraries(Demo PRIVATE sfml-flexui)

set_target_properties(Demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/examples"
)
