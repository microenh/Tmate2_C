cmake_minimum_required(VERSION 3.0.0)
project(gtk_demo VERSION 0.1.0)

set(CMAKE_C_FLAGS "-mno-ms-bitfields")

# Add GTK3+
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)



# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})

FILE(GLOB sources *.c tmate2/*.c)
add_executable(tmate2 ${sources})

target_link_libraries(tmate2
    ${GTK3_LIBRARIES}
    libhidapi             # windows
    # libhidapi-hidraw.so     # linux
)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)