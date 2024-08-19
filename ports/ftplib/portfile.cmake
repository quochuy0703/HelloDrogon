# Chỉ định đường dẫn tới mã nguồn
set(SOURCE_PATH ${CMAKE_CURRENT_LIST_DIR})

# Chạy CMake để cấu hình và xây dựng thư viện từ mã nguồn
vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
)

vcpkg_install_cmake()

vcpkg_copy_pdbs()

vcpkg_fixup_cmake_targets(CONFIG_PATH lib/cmake/ftplib)
