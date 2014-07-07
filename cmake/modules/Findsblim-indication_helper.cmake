
find_path(sblim-indication_helper_INCLUDE_DIR
    NAMES ind_helper.h
    HINTS $ENV{sblim-indication_helper_INCLUDE_DIR}
    PATH_SUFFIXES include/ include
    PATHS /usr /usr/local
)

find_library(sblim-indication_helper_LIBRARY
    NAMES ind_helper
    HINTS $ENV{sblim-indication_helper_LIB_DIR}
    PATH_SUFFIXES lib64 lib
    PATHS /usr /usr/local
)


set(sblim-indication_helper_LIBRARIES ${sblim-indication_helper_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ind_helper DEFAULT_MSG sblim-indication_helper_LIBRARIES sblim-indication_helper_INCLUDE_DIR)

mark_as_advanced(sblim-indication_helper_INCLUDE_DIR sblim-indication_helper_LIBRARIES)
