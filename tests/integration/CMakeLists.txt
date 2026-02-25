message(STATUS "🧪 Configuring integration tests...")

file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(qga_tests_integration ${TEST_SOURCES})

target_include_directories(qga_tests_integration PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/tests
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(qga_tests_integration PRIVATE cxx_std_23)
target_compile_definitions(qga_tests_integration PRIVATE
  QGA_TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/tests/data"
)

target_link_libraries(qga_tests_integration PRIVATE
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
  GTest::gtest
  GTest::gmock
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(qga_tests_integration
  DISCOVERY_TIMEOUT 30
  PROPERTIES LABELS "integration"
)
