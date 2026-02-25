# ==============================================
# QuantGradesApp — Unit Tests (GTest Only)
# ==============================================

message(STATUS "🧪 Configuring unit tests (GTest Only)...")

# === Common settings as a function to avoid duplication (optional) ===

# === Include paths ===
function(qga_apply_unit_test_settings tgt)
    target_include_directories(${tgt} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/tests
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/external
        ${CMAKE_CURRENT_SOURCE_DIR}
    )


    # === Compiler options ===
    target_compile_features(${tgt} PRIVATE cxx_std_23)
    target_compile_definitions(${tgt} PRIVATE
            UNIT_TEST
            USE_CURL
            QGA_UNIT_TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
            QGA_TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/tests/data"
            QGA_TEST_FIXTURES_DIR="${CMAKE_SOURCE_DIR}/tests/fixtures"
    )

    # === Link libraries ===
    target_link_libraries(${tgt} PRIVATE
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

endfunction()

# ==============================================
# GoogleTest Executable
# ==============================================

file(GLOB GTEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*_gtest.cpp")

if(GTEST_SOURCES)
    add_executable(qga_tests_gtest ${GTEST_SOURCES})
    qga_apply_unit_test_settings(qga_tests_gtest)

    target_link_libraries(qga_tests_gtest PRIVATE
        GTest::gtest
        GTest::gmock
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(qga_tests_gtest
        PROPERTIES LABELS "unit;gtest"
    )
    message(STATUS "  [GoogleTest] Target 'qga_tests_gtest' configured.")
else()
    # If no files found, do not create executable - avoid error
    message(WARNING "  [GoogleTest] No *_gtest.cpp files found. Target skipped.")
endif()

add_custom_target(build_unit_tests DEPENDS qga_tests_gtest)

message(STATUS "✅ Unit tests configuration complete.")

# === TO DO in 1.1.5 =====
# Unit tests (if any)
#if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit")
#    add_subdirectory(unit)
#endif()
