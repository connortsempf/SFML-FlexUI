#### Project Configuration ####
## CMake Version Requirement ##
cmake_minimum_required(VERSION 3.16)
## SFML-FlexUI Project Definition ##
project(SFML_FlexUI VERSION 1.0.0 LANGUAGES CXX)
## C++ Version Requirement ##
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)




#### Static or Dynamic Linking Configuration ####
## User Option for Linking Type of SFML Libraries -- Static or Dynamic -- Static by Default ##
if(NOT DEFINED PROJECT_LINK_TYPE)
    set(PROJECT_LINK_TYPE "STATIC" CACHE STRING "Link SFML-FlexUI and SFML: STATIC or DYNAMIC")
endif()
## Validate Static or Dynamic Link Type in Build Command ##
if(NOT PROJECT_LINK_TYPE STREQUAL "STATIC" AND NOT PROJECT_LINK_TYPE STREQUAL "DYNAMIC")
    message(FATAL_ERROR "PROJECT_LINK_TYPE must be STATIC or DYNAMIC")
endif()
## Apply Static or Dynamic Link Type ##
if(PROJECT_LINK_TYPE STREQUAL "STATIC")
    set(SFML_STATIC_LIBRARIES TRUE)
    message(STATUS "Configuring SFML-FlexUI and SFML for Static Linking")
else()
    set(SFML_STATIC_LIBRARIES FALSE)
    message(STATUS "Configuring SFML-FlexUI and SFML for Dynamic Linking")
endif()




#### SFML Configuration ####
## Have CMake look for the SFML Directory for Headers and Libraries ##
find_package(SFML 3.0.2 COMPONENTS System Window Graphics)
## Ensure the SFML CMake Config Directory was Found ##
if(NOT SFML_FOUND)
    message(FATAL_ERROR
        "SFML 3.0.2 or newer was not found!\n"
        "Please install SFML 3.0.2 or newer or provide the path using -DSFML_DIR=/path/to/SFMLConfig.cmake\n"
        "Example: cmake .. -DSFML_DIR=/path/to/SFMLConfig.cmake")
endif()




#### SFUI Configuration ####
## Obtain All Source Code Files from SFML-FlexUI ##
file(GLOB_RECURSE SFML_FLEXUI_SOURCES CONFIGURE_DEPENDS src/*.cpp)
## Create the Library for SFML-FlexUI as a STATIC or DYNAMIC Library based on the User's Build Command ##
if(PROJECT_LINK_TYPE STREQUAL "STATIC")
    add_library(sfml-flexui STATIC ${SFML_FLEXUI_SOURCES})
    message(STATUS "Building SFML-FlexUI as a STATIC Library")
elseif(PROJECT_LINK_TYPE STREQUAL "DYNAMIC")
    add_library(sfml-flexui SHARED ${SFML_FLEXUI_SOURCES})
    message(STATUS "Building SFML-FlexUI as a DYNAMIC Library")
endif()
## Obtain Source Code Header Files from SFML-FlexUI ##
target_include_directories(sfml-flexui PUBLIC include)
## Link the SFML-FlexUI Library to the SFML Libraries ##
target_link_libraries(sfml-flexui PUBLIC SFML::System SFML::Window SFML::Graphics)
## Define SFML_STATIC Macro if Linking Statically ##
if(PROJECT_LINK_TYPE STREQUAL "STATIC")
    target_compile_definitions(sfml-flexui PUBLIC SFML_STATIC)
endif()




#### SFUI Examples Configuration ####
option(BUILD_EXAMPLES "Build Example Programs" ON)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if (WIN32)
    set_target_properties(demo PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
