# ========================================================================
# Custom triplet for QuantGradesApp - Pure MSVC build, no MSYS2
# ========================================================================

set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE static)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_PLATFORM_TOOLSET v143)

# Disable any MSYS2 usage entirely
set(VCPKG_USE_MSYS2 OFF)
set(VCPKG_DOWNLOAD_MSYS2 OFF)
set(VCPKG_POLICY_ALLOW_USING_EXCLUDED_LIBS OFF)

# Pure MSVC environment
set(VCPKG_CMAKE_SYSTEM_NAME Windows)
set(VCPKG_TARGET_IS_WINDOWS ON)
set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "")

# Optional tweaks for speed and determinism
set(VCPKG_BUILD_TYPE release)
set(VCPKG_DEP_INFO_OVERRIDE_VARS "CMAKE_SYSTEM_NAME")
set(VCPKG_MAKE_PROGRAM "C:/ninja/ninja.exe")
