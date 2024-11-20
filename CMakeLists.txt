set(source_files
    model/pseudo-tle.cc
    model/sat-isl-signal.cc
    model/sat-isl-channel.cc
    model/sat-isl-pck-tag.cc
    model/sat-isl-net-device.cc
    model/sat-isl-terminal.cc
    model/sat-isl-antenna.cc
    model/sat-leo-propagation-loss.cc
    model/sat-leo-delay-model.cc
    model/sat-isl-intercon-table.cc
    model/sat-isl-ipv4-routing.cc
#    model/sat-node.cc
    helper/groundstation-helper.cc
    helper/gsl-channel-helper.cc
    helper/walker-constellation-helper.cc
    helper/walker-orbit-helper.cc
    helper/sat-isl-helper.cc
    helper/sat-isl-terminal-helper.cc
    helper/sat-isl-interface-helper.cc
    helper/sat-node-helper.cc
    helper/sat-node-tag.cc
    utils/orientation-helper.cc
    utils/demo-setup-helper.cc
)

set(header_files
    model/sat-isl-def.h
    model/pseudo-tle.h
    model/sat-isl-signal.h
    model/sat-isl-channel.h
    model/sat-isl-pck-tag.h
    model/sat-isl-net-device.h
    model/sat-isl-terminal.h
    model/sat-isl-antenna.h
    model/sat-leo-propagation-loss.h
    model/sat-leo-delay-model.h
    model/sat-isl-intercon-table.h
    model/sat-isl-ipv4-routing.h
#    model/sat-node.h
    helper/groundstation-helper.h
    helper/gsl-channel-helper.h
    helper/walker-constellation-helper.h
    helper/walker-orbit-helper.h
    helper/sat-isl-helper.h
    helper/sat-isl-terminal-helper.h
    helper/sat-isl-interface-helper.h
    helper/sat-node-helper.h
    helper/sat-node-tag.h
    utils/orientation-helper.h
    utils/demo-setup-helper.h
)


set(test_sources
    test/mlxsat-orientation-helper-test.cc
)

build_lib(
    LIBNAME mlxsat
    SOURCE_FILES ${source_files}
    HEADER_FILES ${header_files}
    LIBRARIES_TO_LINK
        ${libcore}
        ${libapplications}
        ${libflow-monitor}
    TEST_SOURCES ${test_sources}
)