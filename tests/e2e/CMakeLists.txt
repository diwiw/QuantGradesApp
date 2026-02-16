# ==============================================
# QuantGradesApp — End-to-End (E2E) Tests
# ==============================================

message(STATUS "🧪 Configuring end-to-end (E2E) tests...")

add_executable(qga_tests_e2e
    ${CMAKE_CURRENT_SOURCE_DIR}/test_main_e2e.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_cli_e2e.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_cli_integration.cpp
)

# ============================================================
# Ensure CLI binary is built before running E2E tests
# ============================================================
add_dependencies(qga_tests_e2e qga_cli)

# ============================================================
# Provide path to CLI binary for E2E tests
# ============================================================

if (CMAKE_CONFIGURATION_TYPES)
    set(_cfg "$<CONFIG>")
else()
    set(_cfg "${CMAKE_BUILD_TYPE}")
endif()

# Copy HTTP test CSV to build directory
file(COPY
    ${CMAKE_SOURCE_DIR}/tests/data/test_http.csv
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

set(QGA_CLI_PATH_FULL
    "${CMAKE_BINARY_DIR}/${_cfg}/bin/qga_cli${CMAKE_EXECUTABLE_SUFFIX}"
)

target_include_directories(qga_tests_e2e PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external
    ${CMAKE_CURRENT_SOURCE_DIR}          # for TestHttpServer.hpp
)

target_link_libraries(qga_tests_e2e PRIVATE
    qga_core
    qga_domain
    qga_strategy
    qga_utils
    qga_io
    qga_ingest
    qga_cli_lib
    CLI11::CLI11
)

# Inject CLI binary path into tests
target_compile_definitions(qga_tests_e2e PRIVATE
    QGA_CLI_PATH="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qga_cli"
    QGA_E2E_HTTP_DIR="${CMAKE_CURRENT_BINARY_DIR}"
    QGA_DATA_DIR="${CMAKE_SOURCE_DIR}/tests/data"
)

add_test(NAME e2e_cli COMMAND qga_tests_e2e)

set_tests_properties(e2e_cli PROPERTIES
    LABELS "e2e"
)

message(STATUS "✅ End-to-end (E2E) tests target configured successfully.")
