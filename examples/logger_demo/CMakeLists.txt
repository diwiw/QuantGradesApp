# ==============================================
# Logger Demo Example (legacy Logger)
# ==============================================

file(GLOB_RECURSE APP_SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(logger_demo
    logger_demo.cpp
)

target_sources(logger_demo PRIVATE
    ${CMAKE_SOURCE_DIR}/src/ingest/DataIngest.cpp
    ${CMAKE_SOURCE_DIR}/src/io/DataExporter.cpp
    ${CMAKE_SOURCE_DIR}/src/domain/backtest/BarSeries.cpp
)

target_include_directories(logger_demo PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/_deps/spdlog-src/include
    ${CMAKE_BINARY_DIR}/_deps/fmt-src/include
    ${CMAKE_BINARY_DIR}/_deps/nlohmann_json-src/include
)

target_link_libraries(logger_demo PRIVATE
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
    CLI11::CLI11
)


target_compile_definitions(logger_demo PRIVATE
    USE_CURL    
)

set_target_properties(logger_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

#add_executable(demo_db_logger_config ${SRC_FILES} ${EXAMPLES_DIR}/demo_db_logger_config.cpp)
	#target_include_directories(demo_db_logger_config PRIVATE ${INCLUDE_DIR})
	#target_link_libraries(demo_db_logger_config PRIVATE nlohmann_json::nlohmann_json CURL::libcurl spdlog::spdlog SQLite::SQLite3)
	#target_compile_definitions(demo_db_logger_config PRIVATE USE_CURL
#		D A T A_P A T H=\"${CMAKE_SOURCE_DIR}/data\"
