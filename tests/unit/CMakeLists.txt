cmake_minimum_required(VERSION 3.25)

# ── config layer ──────────────────────────────────────────────────────────────
modeler_add_test(
    NAME  AppConfigTest
    SOURCES
        config/AppConfigTest.cpp
        ${PROJECT_SOURCE_DIR}/src/config/AppConfig.cpp
)

# ── core layer ────────────────────────────────────────────────────────────────
modeler_add_test(
    NAME  CommandStackTest
    SOURCES
        core/CommandStackTest.cpp
        ${PROJECT_SOURCE_DIR}/src/core/CommandStack.cpp
)
