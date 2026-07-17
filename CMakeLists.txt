cmake_minimum_required(VERSION 3.25)

# If VCPKG_ROOT is available and no explicit toolchain is provided,
# automatically use vcpkg to resolve dependencies.
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE
        "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Path to the vcpkg toolchain file"
    )
endif()

project(Modeler VERSION 0.1.0 LANGUAGES CXX)

# ── Language standard ──────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json for clang-tidy / IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Dependencies ───────────────────────────────────────────────────────────────
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# ── Sources ────────────────────────────────────────────────────────────────────
set(MODELER_SOURCES
    src/window.cpp
    src/config/AppConfig.cpp
    src/core/CommandStack.cpp
    src/ui/render.cpp
    src/ui/sceneAssistent.cpp
    src/ui/menu_buttons/menuButtons.cpp
    src/ui/menu_buttons/projectManager.cpp
    src/ui/menu_buttons/settingsManager.cpp
    src/utils_local/utils_local.cpp
)

# WIN32 suppresses the console window on Windows; ignored on other platforms
add_executable(Modeler WIN32 ${MODELER_SOURCES})

target_include_directories(Modeler PRIVATE
    include                     # public interfaces: modeler/core/, modeler/config/, …
    src                         # window.h
    src/ui                      # render.h, sceneAssistent.h
    src/utils_local             # utils_local.h (legacy)
)

target_link_libraries(Modeler PRIVATE ${wxWidgets_LIBRARIES})

# ── Compiler warnings ──────────────────────────────────────────────────────────
include(cmake/CompilerWarnings.cmake)
set_modeler_warnings(Modeler)

# ── Tests ──────────────────────────────────────────────────────────────────────
option(MODELER_BUILD_TESTS "Build unit tests" ON)
if(MODELER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ── Documentation ─────────────────────────────────────────────────────────────
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    add_custom_target(doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/docs/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()
