message(STATUS "🧪 Configuring SYSTEM tests (Doctest)...")

file(GLOB SYSTEM_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(qga_tests_system_doctest ${SYSTEM_SOURCES})

target_include_directories(qga_tests_system_doctest PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/tests
  ${CMAKE_SOURCE_DIR}/src        # bo AppCLI.hpp jest w src/cli (private)
  ${CMAKE_SOURCE_DIR}/external
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(qga_tests_system_doctest PRIVATE cxx_std_23)

target_compile_definitions(qga_tests_system_doctest PRIVATE
  SYSTEM_TEST
  QGA_TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/tests/data"
  QGA_TEST_FIXTURES_DIR="${CMAKE_SOURCE_DIR}/tests/fixtures"
)

target_link_libraries(qga_tests_system_doctest PRIVATE
  qga_cli          # jeśli masz target biblioteki/komponentu CLI
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

add_test(NAME system_doctest COMMAND qga_tests_system_doctest)
set_tests_properties(system_doctest PROPERTIES LABELS "system;doctest")
