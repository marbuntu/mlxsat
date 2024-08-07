set(source_files
    model/pseudo-tle.cc
    model/sat-static-isl-channel.cc
    model/sat-isl-pck-tag.cc
    model/sat-isl-net-device.cc
    model/sat-isl-terminal.cc
    helper/groundstation-helper.cc
    helper/gsl-channel-helper.cc
    helper/walker-constellation-helper.cc
    helper/walker-orbit-helper.cc
    helper/sat-isl-helper.cc
)

set(header_files
    model/pseudo-tle.h
    model/sat-static-isl-channel.h
    model/sat-isl-pck-tag.h
    model/sat-isl-net-device.h
    model/sat-isl-terminal.h
    helper/groundstation-helper.h
    helper/gsl-channel-helper.h
    helper/walker-constellation-helper.h
    helper/walker-orbit-helper.h
    helper/sat-isl-helper.h
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