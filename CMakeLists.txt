set(source_files
    model/pseudo-tle.cc
    helper/groundstation-helper.cc
    helper/gsl-channel-helper.cc
    helper/walker-constellation-helper.cc
    helper/walker-orbit-helper.cc
)

set(header_files
    model/pseudo-tle.h
    helper/groundstation-helper.h
    helper/gsl-channel-helper.h
    helper/walker-constellation-helper.h
    helper/walker-orbit-helper.h
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