cmake_minimum_required(VERSION 3.18)

project(error LANGUAGES CXX)

# Import third-party dependencies.
find_package(Boost  REQUIRED CONFIG COMPONENTS headers)
find_package(fmt    REQUIRED CONFIG)
find_package(GTest  REQUIRED CONFIG)

# Enable some compiler warnings (supported by gcc & clang).
set(warnings
    -Wall
    -Wconversion
    -Wextra
    -Wformat=2
    -Wold-style-cast
    -Woverloaded-virtual
    -Wshadow
    -Wsign-conversion
    -Wuninitialized
    -Wunused
    )
string(REPLACE ";" " " warnings "${warnings}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings}")

add_library(error SHARED)
add_library(caesar::error ALIAS error)

# Require C++17.
target_compile_features(error PUBLIC cxx_std_17)

# Add sources.
set(sources
    caesar/error/detail/no_value_policy.cpp
    caesar/error/domain_error.cpp
    caesar/error/error_category.cpp
    caesar/error/error_code.cpp
    caesar/error/error.cpp
    caesar/error/expected.cpp
    caesar/error/out_of_range.cpp
    caesar/error/runtime_error.cpp
    )
target_sources(error PRIVATE ${sources})

# Add include dirs.
target_include_directories(
    error
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    )

# Link to imported targets.
target_link_libraries(
    error
    PUBLIC
        Boost::headers
    PRIVATE
        fmt::fmt
    )

# Add test sources.
set(tests
    test/error_code_test.cpp
    test/error_test.cpp
    test/expected_test.cpp
    )

add_executable(error-test ${tests})

target_link_libraries(
    error-test
    PRIVATE
        caesar::error
        GTest::gmock_main
    )

# Register tests with CTest.
enable_testing()
include(GoogleTest)
gtest_discover_tests(error-test)
