
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

typedef std::vector<std::string> StringVector;

extern const char* kTypeNames[];
void HierPrint(std::string const &str, int layer, std::string const &type);
bool documentLint(rapidjson::Document const *json);
void printDocument(char const *name, rapidjson::Value const *doc, int layer);

void findMemberName(rapidjson::Value const *, const std::string name, StringVector &); //with wildcard
void getNameBySplitter(char const *, char const *, StringVector &);

namespace ns3_net
{
	using namespace ns3;
	using namespace ns3_helper;

	const int ipAddressMask[] = { 0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE };

	typedef struct NodesTuple
	{
		uint32_t	id;
		Nodes		nodes;
		Nets		nets;
		Ifaces		ifaces;
	}NodesTuple_t;

	typedef std::vector<NodesTuple> NodesTupleContainer;
	typedef std::map<uint32_t, std::vector<uint32_t>> InterGroupMap; //NodeTypeId-->{NodesTupleId}

	class NetRootTree
	{
	public:
		typedef std::vector<NetRootTree *> pNetChildren;
		typedef std::vector<pNetChildren> pNetChildrenList;
	//construct
		NetRootTree(char const *path);
		NetRootTree(rapidjson::Document *,rapidjson::Value &, rapidjson::Value &, \
			int layer, char const *name);
		~NetRootTree();
		void construct();
	//helper function
		int getLayer() const;
		void HierPrint(char const *str, std::string const &type);
		void printLayers();
		void expand_children(StringVector &Children);
		void expand_config(rapidjson::Value::Array &config, StringVector &Children);
		void expand_template(rapidjson::Value &ref, rapidjson::Value &tmpl);
		std::string getName() const;
		void getNextByIndex(const int, pNetChildren &);
		// void getNextByName(char const *, pNetChildren &); //ABANDON
		void getByGroupName(char const *, pNetChildren &);
	//virtual function
		virtual void applyApplications();

	private:
	//id
		int layer;
		std::string GroupName;
		NodesTupleContainer group;
	//const DOM
		rapidjson::Document* doc;
		rapidjson::Value *topology, *physical;
	//relationship
		InterGroupMap map;		//interconnection
		pNetChildrenList pNext;		//to sublayer
	};

}

#endif
