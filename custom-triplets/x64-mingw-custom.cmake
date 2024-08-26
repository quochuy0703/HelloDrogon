set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)
set(VCPKG_ENV_PASSTHROUGH PATH)
set(VCPKG_BUILD_TYPE release)

set(VCPKG_CMAKE_SYSTEM_NAME MinGW)
if(${PORT} MATCHES  "librdkafka")
    set(VCPKG_LIBRARY_LINKAGE static) # This changed from static to dynamic
endif()
set(VCPKG_POLICY_DLLS_WITHOUT_LIBS enabled)
