cmake_minimum_required(VERSION 3.25)

# ── Fetch GoogleTest ───────────────────────────────────────────────────────────
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
    GIT_SHALLOW    TRUE
)
# Do not install GoogleTest alongside the project
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# ── Helper: create a test executable ──────────────────────────────────────────
# Usage: modeler_add_test(NAME <name> SOURCES <src...>)
function(modeler_add_test)
    cmake_parse_arguments(ARG "" "NAME" "SOURCES" ${ARGN})

    add_executable(${ARG_NAME} ${ARG_SOURCES})

    target_include_directories(${ARG_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
    )

    target_link_libraries(${ARG_NAME}
        PRIVATE
            GTest::gtest_main
            GTest::gmock
    )

    gtest_discover_tests(${ARG_NAME})
endfunction()

# ── Unit tests (no GUI dependency) ────────────────────────────────────────────
add_subdirectory(unit)
