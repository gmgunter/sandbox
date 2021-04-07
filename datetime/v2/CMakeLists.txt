cmake_minimum_required(VERSION 3.19)

project(time LANGUAGES CXX)

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
find_package(date REQUIRED CONFIG COMPONENTS date date-tz)
find_package(doctest REQUIRED CONFIG)
find_package(fmt REQUIRED CONFIG)
find_package(range-v3 REQUIRED CONFIG)

# Enable compiler diagnostic flags.
set(warnings
    -Wall
    -Walloc-zero
    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wcuda-compat
    -Wdeprecated
    -Wdouble-promotion
    -Wduplicated-branches
    -Wduplicated-cond
    -Wextra
    -Wformat=2
    -Wimplicit-fallthrough
    -Winconsistent-missing-destructor-override
    -Wlogical-op
    -Wloop-analysis
    -Wmisleading-indentation
    -Wmissing-noreturn
    -Wnon-virtual-dtor
    -Wnull-dereference
    -Wold-style-cast
    -Woverloaded-virtual
    -Wpacked
    -Wpedantic
    -Wredundant-parens
    -Wshadow
    -Wsign-conversion
    -Wstrict-aliasing
    -Wstrict-overflow
    -Wtautological-compare
    -Wthread-safety
    -Wundef
    -Wundefined-func-template
    -Wundefined-reinterpret-cast
    -Wuninitialized
    -Wunknown-pragmas
    -Wunneeded-internal-declaration
    -Wunreachable-code-aggressive
    -Wunused
    -Wunused-member-function
    -Wvector-conversion
    -Wvla
    -Wweak-template-vtables
    -Wweak-vtables
    -Wzero-as-null-pointer-constant
    )
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

add_library(time SHARED)
add_library(caesar::time ALIAS time)

# Require C++17.
target_compile_features(time PUBLIC cxx_std_17)

# Add sources.
set(sources
    caesar/time/datetime.cpp
    #caesar/time/gpstime.cpp
    caesar/time/timedelta.cpp
    #caesar/time/timepoint.cpp
    #caesar/time/utctime.cpp
    caesar/type_traits.cpp
    )
target_sources(time PRIVATE ${sources})

# Add include dirs.
target_include_directories(
    time
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    )

# Link to imported targets.
target_link_libraries(
    time
    PUBLIC
        absl::numeric
        date::date
        date::date-tz
        range-v3::range-v3
    PRIVATE
        fmt::fmt
    )

if(CAESAR_TEST)
    enable_testing()

    # Add test sources.
    set(tests
        test/time/datetime_test.cpp
        #test/time/gpstime_test.cpp
        test/time/timedelta_test.cpp
        #test/time/timepoint_test.cpp
        #test/time/utctime_test.cpp
        test/type_traits_test.cpp
        test/main.cpp
        )
    add_executable(time-test ${tests})

    target_link_libraries(
        time-test
        PRIVATE
            caesar::time
            doctest::doctest
        )

    # Register tests with CTest.
    include(doctest)
    doctest_discover_tests(time-test)
endif()
