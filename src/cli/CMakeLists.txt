# ============================================================
# 🧩 CLI Layer — QuantGradesApp Command-Line Interface
# ============================================================

# === 1. Build reusable library for CLI logic ===
add_library(qga_cli_lib STATIC
    AppCLI.cpp
)

target_include_directories(qga_cli_lib
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(qga_cli_lib
    PUBLIC
        qga_core
        qga_utils
        qga_io
        qga_ingest
        qga_domain
        qga_strategy
        qga_persistence
        qga_reporting
        CLI11::CLI11
        spdlog::spdlog
)

target_compile_features(qga_cli_lib PUBLIC cxx_std_23)
target_compile_options(qga_cli_lib PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

# === 2. Build CLI executable using the library ===
add_executable(qga_cli main_cli.cpp)

target_link_libraries(qga_cli PRIVATE qga_cli_lib)

# === 3. Optional sanitizers for debug builds ===
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT MSVC)
    target_compile_options(qga_cli PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(qga_cli PRIVATE -fsanitize=address)
endif()
