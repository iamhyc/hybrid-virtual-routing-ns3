## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):
	# The main struct of the simulation
	obj = bld.create_ns3_program('main', 
		['core', 'stats', 'point-to-point', 'csma', 'wifi', 'internet', 'applications'])
	obj.source = 'main.cc'

	# The baseline compared with Dense Wi-Fi Area (User@1st layer)
	obj = bld.create_ns3_program('single-layer-wifi', 
		['core', 'stats', 'point-to-point', 'csma', 'wifi', 'internet', 'applications'])
	obj.source = '01single-layer-wifi.cc'

	# The compared topology with 1 AP to 1 VLC-Rx Bind (User@2nd layer)
	obj = bld.create_ns3_program('vlc-p2p-relay-wifi', 
		['core', 'stats', 'point-to-point', 'csma', 'wifi', 'internet', 'applications'])
	obj.source = '02vlc-p2p-relay-wifi.cc'

	# The compared topology with 1 AP to 1 VLC-Rx Bind (User@1st+2nd layer)
	obj = bld.create_ns3_program('vlc-p2p-relay-wifi-hybrid', 
		['core', 'stats', 'point-to-point', 'csma', 'wifi', 'internet', 'applications'])
	obj.source = '03vlc-p2p-relay-wifi-hybrid.cc'

	# The compared topology with 1 AP to multi-VLC-Rx Bind
	obj = bld.create_ns3_program('vlc-p2m-relay-wifi', 
		['core', 'stats', 'point-to-point', 'csma', 'wifi', 'internet', 'applications'])
	obj.source = '04vlc-p2m-relay-wifi.cc'

	# (Optional) Two-hop Wi-Fi only Relay topology
	obj = bld.create_ns3_program('wifi-p2p-relay-wifi', 
		['core', 'stats', 'point-to-point', 'csma', 'wifi', 'internet', 'applications'])
	obj.source = '05wifi-p2p-relay-wifi.cc'

	pass