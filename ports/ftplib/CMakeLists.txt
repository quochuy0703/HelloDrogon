cmake_minimum_required(VERSION 3.10)
project(ftplib)
set(ftplib "ftplib")

# Thêm source file vào library
add_library(${ftplib} STATIC ftplib.cpp)

# Bao gồm thư mục include cho thư viện
# target_include_directories(${ftplib} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# Bao gồm thư mục include cho thư viện
target_include_directories(${ftplib} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Thêm bất kỳ liên kết thư viện hoặc cài đặt định nghĩa compile cần thiết
target_link_libraries(${ftplib} PUBLIC ws2_32)
target_compile_definitions(${ftplib} PUBLIC NOSSL _WIN32 BUILDING_DLL)

# Định nghĩa target install cho thư viện
install(TARGETS ${ftplib}
        EXPORT ftplibTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

# Cài đặt tệp header vào thư mục include
install(FILES ftplib.h DESTINATION include)

# Tạo file config cho target export
install(EXPORT ftplibTargets
        FILE ftplibConfig.cmake
        DESTINATION lib/cmake/ftplib)
