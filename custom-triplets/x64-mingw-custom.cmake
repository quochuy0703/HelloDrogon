# Detect OS
if(UNIX)
    set(VCPKG_CMAKE_SYSTEM_NAME Linux)
else()
    set(VCPKG_CMAKE_SYSTEM_NAME MinGW)
endif()

# Common settings
set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)
set(VCPKG_ENV_PASSTHROUGH PATH)
set(VCPKG_BUILD_TYPE release)

# Library-specific settings
if(${PORT} MATCHES "librdkafka")
    if(VCPKG_CMAKE_SYSTEM_NAME STREQUAL "Linux")
        # Configure for Linux
        set(VCPKG_LIBRARY_LINKAGE dynamic)
    else()
        # Configure for MinGW
        set(VCPKG_LIBRARY_LINKAGE static)
    endif()
endif()

# Handle policies
set(VCPKG_POLICY_DLLS_WITHOUT_LIBS enabled)
