cmake_minimum_required(VERSION 3.10)
project(network-diagnostic C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find required packages
find_package(CURL REQUIRED)
find_package(Threads REQUIRED)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.c
    src/network.c
    src/server.c
    src/json.c
)

# Create executable
add_executable(network-diagnostic ${SOURCES})

# Link libraries
target_link_libraries(network-diagnostic
    PRIVATE
    CURL::libcurl
    Threads::Threads
    m  # Math library
)

# Compiler flags
if(MSVC)
    target_compile_options(network-diagnostic PRIVATE /W4)
else()
    target_compile_options(network-diagnostic PRIVATE -Wall -Wextra -Werror -O2)
endif()

# Installation
install(TARGETS network-diagnostic DESTINATION bin)
install(DIRECTORY web/ DESTINATION share/network-diagnostic/web)

# CPack configuration
set(CPACK_PACKAGE_NAME "network-diagnostic")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VENDOR "Network Diagnostic")
set(CPACK_PACKAGE_DESCRIPTION "Network Diagnostic Tool - Check your internet speed, IP, and ISP")

include(CPack)
