cmake_minimum_required(VERSION 3.27)
project(QuantGradesApp LANGUAGES CXX)

# ============================================================
# 🧩 Load version info
# ============================================================
include(${CMAKE_SOURCE_DIR}/cmake/version.cmake)

message(STATUS "🧩 QuantGradesApp version: ${QGA_VERSION} (${QGA_GIT_VERSION})")
message(STATUS "🕓 Build date: ${QGA_BUILD_DATE}")

# ============================================================
# ⚙️ Build configuration
# ============================================================

option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)
option(BUILD_DOCS "Build documentation with doxygen" OFF)
option(BUILD_LEGACY_DEMOS "Build legacy demo targets (grades_demo, logger_demo)" OFF)
option(BUILD_API "Build REST API server" OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(MSVC)
    add_compile_options(/W4 /permissive-)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(CTest)

# ============================================================
# 🧩 Optional sanitizer
# ============================================================
if (ENABLE_TSAN)
    message(STATUS "🔬 ThreadSanitizer enabled")
    add_compile_options(-fsanitize=thread -fno-omit-frame-pointer -O1)
    add_link_options(-fsanitize=thread)
endif()

# ============================================================
# 🧩 Ensure required dirs exist
# ============================================================
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR})
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/generated)

# ============================================================
# 📁 Directory structure
# ============================================================
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set(EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/examples)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
set(DATA_DIR ${CMAKE_SOURCE_DIR}/data)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(${INCLUDE_DIR} ${EXTERNAL_DIR})

# ============================================================
# 🧱 Version.hpp (auto-generated from template)
# ============================================================
set(VERSION_TEMPLATE ${CMAKE_SOURCE_DIR}/cmake/templates/version.h.in)
set(VERSION_HEADER ${CMAKE_BINARY_DIR}/generated/Version.hpp)

# We use configure_file — simple and works in CI
configure_file(
    ${VERSION_TEMPLATE}
    ${VERSION_HEADER}
    @ONLY
)

include_directories(${CMAKE_BINARY_DIR}/generated)
message(STATUS "📦 Generated Version.hpp → ${VERSION_HEADER}")

# ============================================================
# 🧱 CCache
# ============================================================
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  message(STATUS "ccache found: ${CCACHE_PROGRAM}")
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
else()
  message(STATUS "ccache not found — building without compiler cache")
endif()

# ============================================================
# 📦 Dependencies (vcpkg)
# ============================================================

include(FetchContent)

# ---- fmt ----------------------------------------------------
find_package(fmt CONFIG QUIET)

if(NOT fmt_FOUND)
    message(STATUS "fmt not found via CONFIG, trying system / FetchContent")

    include(FetchContent)
    FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 12.1.0
    )
    FetchContent_MakeAvailable(fmt)
endif()

# ---- spdlog -------------------------------------------------
find_package(spdlog CONFIG QUIET)

if(NOT spdlog_FOUND)
    message(STATUS "spdlog not found via CONFIG, trying system / FetchContent")

    include(FetchContent)
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG 1.17.0
    )
    FetchContent_MakeAvailable(spdlog)
endif()

# ---- CURL ---------------------------------------------------
find_package(CURL REQUIRED)

# ---- SQLite3 -----------------------------------------------
find_package(SQLite3 REQUIRED)

# ---- nlohmann_json (system / vcpkg / fallback) --------------
find_package(nlohmann_json CONFIG QUIET)

if(NOT nlohmann_json_FOUND)
    message(STATUS "nlohmann_json not found, using FetchContent fallback")
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.11.3
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()

# ---- CLI11 (system / vcpkg / fallback) ----------------------
find_package(CLI11 CONFIG QUIET)

if(NOT CLI11_FOUND)
    message(STATUS "CLI11 not found, using FetchContent fallback")
    FetchContent_Declare(
        CLI11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG        v2.6.1
    )
    FetchContent_MakeAvailable(CLI11)
endif()

# ---- cpp-httplib (header-only, always FetchContent) ---------
# NOTE: cpp-httplib is header-only and does not provide
#       reliable system CMake packages on Linux.
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG        v0.15.3
)
FetchContent_MakeAvailable(httplib)

# ============================================================
# 🧩 Subdirectories (modular build)
# ============================================================
add_subdirectory(${SRC_DIR}/utils)
add_subdirectory(${SRC_DIR}/core)
add_subdirectory(${SRC_DIR}/io)
add_subdirectory(${SRC_DIR}/persistence)
add_subdirectory(${SRC_DIR}/ingest)
add_subdirectory(${SRC_DIR}/strategy)
add_subdirectory(${SRC_DIR}/reporting)
add_subdirectory(${SRC_DIR}/domain)
add_subdirectory(${SRC_DIR}/cli)
add_subdirectory(${SRC_DIR}/api)

# ============================================================
# 🧩 Optional REST API Server
# ============================================================
if (BUILD_API)
    message(STATUS "🌐 Building REST API server")
    add_subdirectory(${SRC_DIR}/apps/qga_api)
endif()

# ============================================================
# 💡 Examples
# ============================================================
add_subdirectory(${EXAMPLES_DIR}/backtest_demo)

if(BUILD_LEGACY_DEMOS)
    add_subdirectory(${EXAMPLES_DIR}/grades_demo)
    add_subdirectory(${EXAMPLES_DIR}/logger_demo)
endif()

# ============================================================
# 🧪 Tests
# ============================================================
enable_testing()
add_subdirectory(tests)

# ============================================================
# 📘 Doxygen
# ============================================================
if(BUILD_DOCS)
    find_package(Doxygen QUIET)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/Doxyfile)
        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "📘 Generating API documentation with Doxygen"
        )
    else()
        message(FATAL_ERROR "❌ Doxygen required when BUILD_DOCS=ON.")
    endif()
endif()

# ============================================================
# 🧹 Clean target
# ============================================================
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/data
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/generated
    COMMENT "🧹 Cleaning build artifacts..."
)

# ============================================================
# 📦 INSTALL / PACKAGING (runtime bundle)
# ============================================================

include(GNUInstallDirs)

# Main binary (Linux / Windows aut)
install(
    TARGETS qga_api RUNTIME DESTINATION .
)

# Runtime layout (copied from repo/runtime → bundle/quantumgrades)
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/runtime/config
    DESTINATION .
)

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/runtime/data
    DESTINATION .
)

install(
    FILES ${CMAKE_SOURCE_DIR}/runtime/README.md
    DESTINATION .
)
