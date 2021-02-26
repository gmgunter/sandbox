cmake_minimum_required(VERSION 3.18)

project(ellipsoid LANGUAGES CXX)

# Import third-party dependencies.
find_package(GTest REQUIRED CONFIG)

# Enable some compiler warnings (supported by gcc & clang).
set(warnings
    -Wall
    -Wconversion
    -Werror
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

add_library(ellipsoid SHARED)
add_library(caesar::ellipsoid ALIAS ellipsoid)

# Require C++17.
target_compile_features(ellipsoid PUBLIC cxx_std_17)

# Add sources.
set(sources
    caesar/spheroid.cpp
    )
target_sources(ellipsoid PRIVATE ${sources})

# Add include dirs.
target_include_directories(
    ellipsoid
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    )

# Add test sources.
set(tests
    test/spheroid_test.cpp
    )

add_executable(ellipsoid-test ${tests})

target_link_libraries(
    ellipsoid-test
    PRIVATE
        caesar::ellipsoid
        GTest::gtest_main
    )

# Register tests with CTest.
enable_testing()
include(GoogleTest)
gtest_discover_tests(ellipsoid-test)
