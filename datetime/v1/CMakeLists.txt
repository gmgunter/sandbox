cmake_minimum_required(VERSION 3.19)

project(datetime LANGUAGES CXX)

# Add custom CMake modules to module include path.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

# Check if this is the top-level CMake project or was included as a sub-project.
if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
    set(CAESAR_MAIN_PROJECT ON)
else()
    set(CAESAR_MAIN_PROJECT OFF)
endif()

# Project configuration options.
option(CAESAR_TEST "Enable test suite" ${CAESAR_MAIN_PROJECT})
option(CAESAR_WERROR "Treat compiler warnings as errors" ON)

# Import third-party dependencies.
find_package(absl REQUIRED CONFIG COMPONENTS numeric)
#find_package(date REQUIRED CONFIG COMPONENTS date date-tz)
find_package(doctest REQUIRED CONFIG)
find_package(fmt REQUIRED CONFIG)

# Enable compiler diagnostic flags.
include(CaesarParseFile)
caesar_parse_file(warnings.txt warnings)

if(CAESAR_WERROR)
    list(APPEND warnings -Werror)
endif()

include(CheckCompilerFlag)
foreach(warning ${warnings})
    check_compiler_flag(CXX ${warning} cxx_${warning}_supported)
    if(cxx_${warning}_supported)
        add_compile_options($<$<COMPILE_LANGUAGE:CXX>:${warning}>)
    endif()
endforeach()

add_library(datetime SHARED)
add_library(caesar::datetime ALIAS datetime)

# Require C++17.
target_compile_features(datetime PUBLIC cxx_std_17)

# Add sources.
include(CaesarParseFile)
caesar_parse_file(sources.txt sources)
target_sources(datetime PRIVATE ${sources})

# Add include dirs.
target_include_directories(
    datetime
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    )

# Link to imported targets.
target_link_libraries(
    datetime
    PUBLIC
        absl::numeric
#        date::date
#        date::date-tz
    PRIVATE
        fmt::fmt
    )

if(CAESAR_TEST)
    enable_testing()

    # Add test sources.
    caesar_parse_file(tests.txt tests)
    add_executable(datetime-test ${tests})

    target_link_libraries(
        datetime-test
        PRIVATE
            caesar::datetime
            doctest::doctest
        )

    # Register tests with CTest.
    include(doctest)
    doctest_discover_tests(datetime-test)
endif()
