# ==============================================
# QuantGradesApp — Tests
# ==============================================

message(STATUS "🧪 Configuring tests...")

# ==== GoogleTest dependency (prefer vcpkg/system, fallback to FetchContent) ====
find_package(GTest CONFIG QUIET)

if(NOT GTest_FOUND)
    message(STATUS "GTest not found via package manager, fetching via CMake FetchContent...")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.14.0
    )

    # Avoid CRT mismatch on MSVC
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    # Normalize target names to always use GTest::*
    if(TARGET gtest_main AND NOT TARGET GTest::gtest_main)
        add_library(GTest::gtest_main ALIAS gtest_main)
    endif()
    if(TARGET gmock AND NOT TARGET GTest::gmock)
        add_library(GTest::gmock ALIAS gmock)
    endif()
endif()

option(QGA_BUILD_TESTS_UNIT "Build unit tests" ON)
option(QGA_BUILD_TESTS_INTEGRATION "Build integration tests" ON)
option(QGA_BUILD_TESTS_SYSTEM "Build system tests" OFF)
# Turned off E2E for now, because it has to be done with google extended test framework.
# Now we would work for windows on separate commands to test server http
option(QGA_BUILD_TESTS_E2E "Build e2e tests" OFF)
option(QGA_BUILD_TESTS_PERF "Build perf tests" OFF)

if(QGA_BUILD_TESTS_UNIT)
  add_subdirectory(unit)
endif()

if(QGA_BUILD_TESTS_INTEGRATION AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/integration")
  if(WIN32)
    message(STATUS "⚠️ Integration tests disabled on Windows for now (HTTP helper is Linux-only).")
  else()
    add_subdirectory(integration)
  endif()
endif()

if(QGA_BUILD_TESTS_SYSTEM AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/system")
  add_subdirectory(system)
endif()

if(QGA_BUILD_TESTS_E2E AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/e2e")
  add_subdirectory(e2e)
endif()

if(QGA_BUILD_TESTS_PERF AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/perf")
  add_subdirectory(perf)
endif()
