
#ifndef __NS3_NET_H__
#define __NS3_NET_H__

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"
// #include "rapidjson/pointer.h"

#include <fstream>
#include <vector>
#include <boost/algorithm/string.hpp>

using namespace std;

const char* kTypeNames[] = 
	{ "Null", "False", "True", "Object", "Array", "String", "Number" };
bool documentLint(rapidjson::Document const *json);
void printDocument(char const *name="", rapidjson::Value const *doc=0, int layer=0);

void findMemberName(rapidjson::Value const *, const string name, vector<string> *); //with wildcard
void getNameBySplitter(const char* message, const char* splitter, vector<string> &tokens)
{
	/*e.g. getNamebySplitter("ipBase", "_", tokens); ("device", "-", tokens)*/
	boost::split(tokens, message, boost::is_any_of(splitter));
}

namespace ns3_net
{
	using namespace ns3;

	const int p2pAddressMask[] = { 0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE };
	const char* kChannelNames[] = { "csma", "wifi", "p2p" };

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
		NetRootTree(rapidjson::Document);
		~NetRootTree();
		static const NetRootTree* getByGroupName(const NetRootTree*, const char *);
		string getName();
		NetRootTree *getNext();

	private:
		string GroupName;
		NodeContainer group;
		NetRootTree *next;
	};

}

#endif
