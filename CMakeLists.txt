
cmake_minimum_required(VERSION 2.6)

# Set flags and definitions
# -std=c99 -> Standard to which the code should conform, C99 standard.
# -pedantic -> Issue all the warnings demanded by strict ISO C and ISO C++.
# -Wno-unused-parameter -> Avoid warning about an unused function parameter.
# -Wformat -> Check calls to "printf" and "scanf" to check whether the format string specified is good.
# -Wparentheses -> Warn if parentheses are omitted in certain contexts
# -Wl,--no-undefined -> Pass --no-undefined as an option to the linker. 
set(CMAKE_C_FLAGS "-std=c99 -Wall -pedantic -g -Wextra -Wno-unused-parameter -Wformat -Wparentheses -Wl,--no-undefined")

# Set LIB_SUFFIX to 64 on 64bit architectures
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(LIB_SUFFIX "")
else(CMAKE_SIZEOF_VOID_P EQUAL 4)
    SET(LIB_SUFFIX 64)
endif(CMAKE_SIZEOF_VOID_P EQUAL 4)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

find_package(PkgConfig)
find_package(CMPI REQUIRED)
find_package(KonkretCMPI REQUIRED)
find_package(sblim-indication_helper REQUIRED)

add_subdirectory(src)
add_subdirectory(mof)
