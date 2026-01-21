# ==============================================
# QuantGradesApp — Unit Tests
# ==============================================

message(STATUS "🧪 Configuring unit tests...")

# === Test sources ===
file(GLOB TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# === Create test executable ===
add_executable(qga_tests
    ${TEST_SOURCES}
)

# === Include paths ===
target_include_directories(qga_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# === Compiler options ===
target_compile_features(qga_tests PRIVATE cxx_std_23)
target_compile_definitions(qga_tests PRIVATE
        UNIT_TEST
        USE_CURL
        QGA_UNIT_TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

# === Link libraries ===
target_link_libraries(qga_tests PRIVATE
    qga_utils
    qga_core
    qga_io
    qga_domain
    qga_ingest
    qga_persistence
    qga_reporting
    qga_strategy
    qga_api_lib
    fmt::fmt
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    CURL::libcurl
    SQLite::SQLite3

)
# === TO DO in 1.1.5 =====
# Unit tests (if any)
#if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit")
#    add_subdirectory(unit)
#endif()

# === Register in CTest (optional, for future CI) ===
add_test(NAME QuantGradesAppTests COMMAND qga_tests)

set_tests_properties(QuantGradesAppTests PROPERTIES
  LABELS "unit;fast"
)

message(STATUS "✅ Unit tests target configured successfully.")
