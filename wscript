## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):
	# The main struct of the simulation
	obj = bld.create_ns3_program('main', 
		['core', 'network', 'stats', 'point-to-point', 'csma', 'wifi', 'internet', 'applications'])
	obj.source = [
		"main.cc", "ns3_net.cc", "ns3_helper.cc", "ns3_perf.cc"
	]

	headers = bld(features='ns3header')
	headers.module = 'main'
	headers.source = [
		"ns3_net.h",
		"ns3_helper.h",
		"ns3_perf.h",
		"include/rapidjson/rapointer.h",
		"include/rapidjson/document.h",
		"include/rapidjson/istreamwrapper.h",
	]
	pass