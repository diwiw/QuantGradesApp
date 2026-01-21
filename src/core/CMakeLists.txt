# ============================================================
# 🧠 qga_core — Application logic and shared data structures
# ============================================================

add_library(qga_core STATIC
    Config.cpp
    ConfigLocator.cpp
    AssetsLocator.cpp
    Bootstrap.cpp
    Statistics.cpp
    # add new core sources explicitly here
)

add_library(qga::core ALIAS qga_core)

target_include_directories(qga_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(qga_core
    PUBLIC
        qga_utils
        nlohmann_json::nlohmann_json
)

target_compile_features(qga_core PUBLIC cxx_std_23)

set_target_properties(qga_core PROPERTIES
    OUTPUT_NAME "qga_core"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

install(TARGETS qga_core
    ARCHIVE DESTINATION lib
)