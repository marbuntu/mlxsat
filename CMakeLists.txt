set(source_files
    helper/sat-orbit-helper.cc
)

set(header_files
    helper/sat-orbit-helper.h
)

build_lib(
    LIBNAME mlxsat
    SOURCE_FILES ${source_files}
    HEADER_FILES ${header_files}
    LIBRARIES_TO_LINK
        ${libcore}
        ${libapplications}
        ${libflow-monitor}
)