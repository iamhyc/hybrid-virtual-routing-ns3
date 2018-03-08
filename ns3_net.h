
#ifndef __NS3_NET_H__
#define __NS3_NET_H__

#include "ns3/core-module.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
#include "rapidjson/pointer.h"

#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>

static const char* kTypeNames[] = 
	{ "Null", "False", "True", "Object", "Array", "String", "Number" };
bool documentLint(rapidjson::Document const *json);
void printDocument(char const *name="", rapidjson::Value const *doc=0, int layer=0);

void findMemberName(rapidjson::Value const *, const string name, std::vector<std::string> *); //with wildcard
void getNameBySplitter(const char* message, const char* splitter, std::vector<std::string> &tokens)
{
	/*e.g. getNamebySplitter("ipBase", "_", tokens); ("device", "-", tokens)*/
	boost::split(tokens, message, boost::is_any_of(splitter));
}

namespace ns3_net
{
	const char* p2pAddressMask[8] = { 127, 191, 223, 239, 247, 251, 253, 254 };
	const char* kChannelNames[] = { "csma", "wifi", "p2p" };

	typedef struct wifiSchema
	{
		char* ssid;
		char* standard;//IEEE 802.11
		int[4] mobility;
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
		NetRootTree(rapidjson::Document);
		~NetRootTree();
		static const *NetRootTree getByGroupName(const *NetRootTree, const char *);
		string getName();
		NetRootTree *getNext();

	private:
		string GroupName;
		ns3::NodesContainner group;
		NetRootTree *next;
	};

}

#endif
