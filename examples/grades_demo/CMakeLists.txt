# ==============================================
# Grades Demo Example (using SpdLogger)
# ==============================================

file(GLOB_RECURSE APP_SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(grades_demo
    grades_demo.cpp
    Grades.cpp
)

target_include_directories(grades_demo PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(grades_demo PRIVATE
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

target_compile_definitions(grades_demo PRIVATE
    USE_CURL    
)

set_target_properties(grades_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
