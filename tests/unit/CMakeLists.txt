# ==============================================
# QuantGradesApp — Unit Tests
# ==============================================

message(STATUS "🧪 Configuring unit tests...")

# === Common settings as a function to avoid duplication (optional) ===

# === Include paths ===
function(qga_apply_unit_test_settings tgt)
    target_include_directories(${tgt} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/external
        ${CMAKE_CURRENT_SOURCE_DIR}
    )


    # === Compiler options ===
    target_compile_features(qga_tests_doctest PRIVATE cxx_std_23)
    target_compile_definitions(qga_tests_doctest PRIVATE
            UNIT_TEST
            USE_CURL
            QGA_UNIT_TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    )

    # === Link libraries ===
    target_link_libraries(qga_tests_doctest PRIVATE
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
# A) Doctest (legacy executable)
# ==============================================

# === Test sources ===
file(GLOB DOCTEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# === Exclude gtest files, if any ===
list(FILTER DOCDOCTEST_SOURCES EXCLUDE REGEX ".*_gtest\\*.cpp$")

# === Create test executable ===
add_executable(qga_tests_doctest
    ${DOCTEST_SOURCES}
)

qga_apply_unit_test_settings(qga_tests_doctest)

add_test(NAME unit_doctest COMMAND qga_tests_doctest)


message(STATUS "✅ Unit tests target configured successfully.")


# ==============================================
# B) GoogleTest (new executable)
# ==============================================

add_executable(qga_tests_gtest
    test_file_manage_gtest.cpp
)

qga_apply_unit_test_settings(qga_tests_gtest)

target_link_libraries(qga_tests_gtest PRIVATE
    GTest::gtest_main
    GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(qga_tests_gtest
    PROPERTIES LABELS "unit;gtest"
)

message(STATUS "✅ GoogleTest Unit tests target configured successfully.")








# === TO DO in 1.1.5 =====
# Unit tests (if any)
#if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit")
#    add_subdirectory(unit)
#endif()

# === Register in CTest (optional, for future CI) ===
add_test(NAME unit_doctest COMMAND qga_tests_doctest)

set_tests_properties(unit_doctest PROPERTIES
  LABELS "unit;doctest"
)

message(STATUS "✅ Unit tests target configured successfully.")
