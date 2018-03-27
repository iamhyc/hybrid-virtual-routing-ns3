
#ifndef __NS3_NET_H__
#define __NS3_NET_H__

#include "ns3_helper.h"
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
	using namespace ns3_helper;

	const int p2pAddressMask[] = { 0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE };
	extern const char* kChannelNames[];

	typedef struct NodeTuple
	{
		Nodes	nodes;
		Nets	nets;
		Ifaces	ifaces;
	}NodeTuple_t;

	class NetRootTree
	{
	public:
		NetRootTree(char const *path, char const *name="root");
		~NetRootTree();
		void printLayers();
		string getName() const;
		NetRootTree const *getNext() const;
		NetRootTree const *getByGroupName(NetRootTree const *, char const *);
		virtual void applyApplications();

	private:
		rapidjson::Document json;
		string GroupName;
		NodeContainer group;
		NetRootTree *next;
	};

}

#endif
