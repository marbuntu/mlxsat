## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):
    obj = bld.create_ns3_module('mlxsat',['core','network','stats','internet','applications'])
    obj.source = [
        'model/pseudo-tle.cc',
        'helper/walker-orbit-helper.cc',
        'helper/walker-constellation-helper.cc'
        ]

    module_test = bld.create_ns3_module_test_library('mlxsat')
    module_test.source = [
        ]

    headers = bld(features='ns3header')
    headers.module = 'mlxsat'
    headers.source = [
        'model/pseudo-tle.h',
        'helper/walker-orbit-helper.h',
        'helper/walker-constellation-helper.h'
        ]
        
    if (bld.env['ENABLE_EXAMPLES']):
        bld.recurse('examples')
