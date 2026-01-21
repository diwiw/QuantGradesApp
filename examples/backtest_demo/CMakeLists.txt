# ==============================================
# Backtest Demo (production-style example)
# ==============================================

file(GLOB_RECURSE BACKTEST_SRC
    ${CMAKE_SOURCE_DIR}/src/domain/backtest/*.cpp
    ${CMAKE_SOURCE_DIR}/src/strategy/*.cpp
    ${CMAKE_SOURCE_DIR}/src/io/*.cpp
    ${CMAKE_SOURCE_DIR}/src/persistence/*.cpp
)

add_executable(backtest_demo
    backtest_demo.cpp
    ${BACKTEST_SRC}
)

target_include_directories(backtest_demo PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(backtest_demo PRIVATE
    qga_core
    qga_utils
    qga_io
    qga_ingest
    qga_domain
    qga_strategy
    qga_persistence
    qga_reporting
    fmt::fmt
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    CURL::libcurl
    SQLite::SQLite3
)


target_compile_definitions(backtest_demo PRIVATE
    USE_CURL    
)

set_target_properties(backtest_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

message(STATUS "✅ Configured backtest_demo (temporary full source inclusion)")


#    add_executable(examples_backtest ${SRC_FILES} ${EXAMPLES_DIR}/main_backtest.cpp)
#	target_include_directories(examples_backtest PRIVATE ${INCLUDE_DIR})
#	target_link_libraries(examples_backtest PRIVATE nlohmann_json::nlohmann_json CURL::libcurl spdlog::spdlog SQLite::SQLite3)
	#target_compile_definitions(examples_backtest PRIVATE USE_CURL)
