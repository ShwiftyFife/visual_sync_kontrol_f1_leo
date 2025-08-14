cmake_minimum_required(VERSION 3.10)
project(traktor CXX)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find HIDAPI
find_path(HIDAPI_INCLUDE_DIR hidapi/hidapi.h)
find_library(HIDAPI_LIBRARY NAMES hidapi-libusb hidapi)

if(NOT HIDAPI_INCLUDE_DIR OR NOT HIDAPI_LIBRARY)
    message(FATAL_ERROR "hidapi not found. Install libhidapi-dev.")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HIDAPI
    DEFAULT_MSG
    HIDAPI_LIBRARY
    HIDAPI_INCLUDE_DIR)

# Add executable
add_executable(traktor main.cpp)

# Include path for compiler
target_include_directories(traktor PRIVATE ${HIDAPI_INCLUDE_DIR})

# Link the HIDAPI library
target_link_libraries(traktor PRIVATE ${HIDAPI_LIBRARY})