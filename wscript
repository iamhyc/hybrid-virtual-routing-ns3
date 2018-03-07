## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):
	# The main struct of the simulation
	obj = bld.create_ns3_program('hvr-main', 
		['core', 'network', 'stats', 'point-to-point', 'csma', 'wifi', 'internet', 'applications'])
	obj.source = [
		"main.cc"
	]

	headers = bld(features='ns3header')
	headers.module = 'hvr-main'
	headers.source = [
		"main.h",
		"ns3_helper.h",
		"ns3_perf.h",
		"include/rapidjson/pointer.h"
		"include/rapidjson/document.h",
		"include/rapidjson/istreamwrapper.h",
	]
	pass