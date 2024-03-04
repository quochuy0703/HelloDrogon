cmake_minimum_required(VERSION 3.5)
project(HelloDrogon CXX)

set(CMAKE_CXX_STANDARD 20)
include(CheckIncludeFileCXX)

check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(coroutine HAS_COROUTINE)
if (NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
    # Do nothing
elseif (HAS_ANY AND HAS_STRING_VIEW AND HAS_COROUTINE)
    set(CMAKE_CXX_STANDARD 20)
elseif (HAS_ANY AND HAS_STRING_VIEW)
    set(CMAKE_CXX_STANDARD 17)
else ()
    set(CMAKE_CXX_STANDARD 14)
endif ()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} main.cc)

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# add_subdirectory(drogon) 
# target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
#
# and comment out the following lines
find_package(Drogon CONFIG REQUIRED)
include_directories(${PROJECT_NAME} "C:\\SoftwareVNN\\msys64\\usr\\local\\include" "C:\\SoftwareVNN\\LT\\lib\\libftp\\include" "C:\\SoftwareVNN\\LT\\lib\\mailio\\include" "C:\\SoftwareVNN\\msys64\\mingw64\\include\\botan-3")
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon botan-3 "C:\\SoftwareVNN\\msys64\\usr\\local\\lib\\libcryptopp.a" ws2_32 "C:\\SoftwareVNN\\msys64\\usr\\local\\lib\\libftp.dll.a" "C:\\SoftwareVNN\\LT\\lib\\mailio\\lib\\libmailio.dll.a")

# ##############################################################################

if (CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR "c++17 or higher is required")
elseif (CMAKE_CXX_STANDARD LESS 20)
    message(STATUS "use c++17")
else ()
    message(STATUS "use c++20")
endif ()

aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FILTER_SRC)
aux_source_directory(plugins PLUGIN_SRC)
aux_source_directory(models MODEL_SRC)
aux_source_directory(utils UTIL_SRC)
aux_source_directory(errors ERROR_SRC)
aux_source_directory(services SERVICE_SRC)
aux_source_directory(dto DTO_SRC)
aux_source_directory(repository REPOSITORY_SRC)
aux_source_directory(plugins PLUGIN_SRC)

# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR})
# use the following line to create views with namespaces.
drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
                    ${CMAKE_CURRENT_BINARY_DIR} TRUE)
# use the following line to create views with namespace CHANGE_ME prefixed
# and path namespaces.
# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE CHANGE_ME)

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                                   ${CMAKE_CURRENT_SOURCE_DIR}/models)
target_sources(${PROJECT_NAME}
               PRIVATE
               ${SRC_DIR}
               ${CTL_SRC}
               ${FILTER_SRC}
               ${PLUGIN_SRC}
               ${MODEL_SRC}
               ${UTIL_SRC}
               ${ERROR_SRC}
               ${SERVICE_SRC}
               ${DTO_SRC}
               ${REPOSITORY_SRC}
               ${PLUGIN_SRC})
# ##############################################################################
# uncomment the following line for dynamically loading views 
# set_property(TARGET ${PROJECT_NAME} PROPERTY ENABLE_EXPORTS ON)

# ##############################################################################

add_subdirectory(test)
