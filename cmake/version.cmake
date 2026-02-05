# ============================================================
# 🌍 Project Version Definition
# ============================================================
set(QGA_VERSION_MAJOR 1)
set(QGA_VERSION_MINOR 0)
set(QGA_VERSION_PATCH 1)

set(QGA_VERSION "${QGA_VERSION_MAJOR}.${QGA_VERSION_MINOR}.${QGA_VERSION_PATCH}")

# Use compilation date
string(TIMESTAMP QGA_BUILD_DATE "%Y-%m-%d %H:%M:%S")

# Git Version (if available)
execute_process(
    COMMAND git describe --tags --always --dirty
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE QGA_GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(NOT QGA_GIT_VERSION)
    set(QGA_GIT_VERSION "v${QGA_VERSION}")
endif()
