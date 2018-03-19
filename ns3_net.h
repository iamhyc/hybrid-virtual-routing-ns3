
#ifndef __NS3_NET_H__
#define __NS3_NET_H__

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "include/rapidjson/istreamwrapper.h"
#include "include/rapidjson/document.h"
#include "include/rapidjson/rapointer.h"

#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>

using namespace std;

extern const char* kTypeNames[];
bool documentLint(rapidjson::Document const *json);
void printDocument(char const *name, rapidjson::Value const *doc, int layer);

void findMemberName(rapidjson::Value const *, const string name, vector<string> *); //with wildcard
void getNameBySplitter(char const *, char const *, vector<string> &);

namespace ns3_net
{
	using namespace ns3;

	const int p2pAddressMask[] = { 0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE };
	extern const char* kChannelNames[];

	typedef struct wifiSchema
	{
		char* ssid;
		char* standard;//IEEE 802.11
		int mobility[4];
		int channel;
		int bandwidth;
	}wifiSchema_t;

	typedef struct generalSchema
	{
		float throughput;
		float delay;
	}generalSchema_t;

	class NetRootTree
	{
	public:
		NetRootTree(char const *path);
		~NetRootTree();
		const NetRootTree *getByGroupName(NetRootTree const *, char const *);
		string getName() const;
		NetRootTree const *getNext() const;

	private:
		string GroupName;
		NodeContainer group;
		NetRootTree *next;
	};

}

#endif
