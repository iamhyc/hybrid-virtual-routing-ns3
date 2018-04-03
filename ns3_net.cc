
#include "ns3_net.h"

using namespace std;
using namespace rapidjson;

char build_log[255];
const char* kTypeNames[] = 
	{ "Null", "False", "True", "Object", "Array", "String", "Number" };
map<string, int> PrintHelper=
{
		{"default",		-1},
		{"build",		0},
		{"inline",		1}
};

void getNameBySplitter(char const *message, char const *splitter, StringVector &tokens)
{
	/*e.g. getNamebySplitter("ipBase", "_", tokens); ("device", "-", tokens)*/
	boost::split(tokens, message, boost::is_any_of(splitter));
}

bool documentLint(bool flag, Document const &json)
{
	assert(flag == 0);
	assert(json["topology"].IsObject());
	assert(json["physical"].IsObject());
	assert(json["application"].IsObject());
	return true;
}

void printDocument(char const *name, Value const *doc, int layer=0)
{
	string m_indent(layer, '\t');
	if (string(name).compare("") != 0)
	{
		printf("%s%s: \n", m_indent.c_str(), name);
	}

	for(auto& m : doc->GetObject())
	{
		auto m_type = m.value.GetType();
		switch(m_type)
		{
			case 3: //JSON Object
				printDocument(m.name.GetString(), &m.value, layer+1);
				break;
			case 4: //Array
				if (m.value.Capacity()>0 && m.value[0].IsObject())
				{
					printf("%s\t\"%s\" [\n", m_indent.c_str(), m.name.GetString());
					printDocument("", &m.value[0], layer+1);
					printf("%s\t]\n", m_indent.c_str());
				}
				break;
			default:
				printf("\t%s\"%s\" is %s\n", m_indent.c_str(), m.name.GetString(), kTypeNames[m_type]);
				break;
		}
	}
	return;
}

void findMemberName(Value const *doc, const std::string name, StringVector &output)
{
	for(auto& m : doc->GetObject())
	{
		string tmp(m.name.GetString());
		if (tmp.find(name) != std::string::npos)
		{
			output.push_back(tmp);
		}
	}
}

using namespace ns3_net;

NetRootTree::~NetRootTree()
{
	if(this->GroupName.compare("root")==0)
	{
		HierPrint("__root_exit__", "inline");
		for(auto it=this->pNext.begin(); it<this->pNext.end();++it)
		{
			delete &(*it);
		}
	}
	else
	{
		sprintf(build_log, "__%s_destruct__", this->GroupName.c_str());
		HierPrint(build_log, "inline");
	}
}

NetRootTree::NetRootTree(char const *path):
	layer(0), GroupName("root")
{
	ifstream ifs(path, fstream::in);
	IStreamWrapper isw(ifs);
	// Load JSON from file
	Document document;
	auto flag = (document).ParseStream(isw).HasParseError();
	documentLint(flag, document);
	//init
	this->doc = move(&document);
	this->topology = (*this->doc)["topology"];
	this->physical = (*this->doc)["physical"];
	construct();
}

NetRootTree::NetRootTree(Document *doc, Value &topo, Value &phy, int layer, char const *name):
	layer(layer), GroupName(name)
{
	this->doc = doc;
	this->topology = topo;
	this->physical = phy;
	construct();
}

void NetRootTree::construct()
{
	// int count = 0;
	StringVector strs;
	findMemberName(&this->topology, "node", strs);

	HierPrint(this->GroupName.c_str(), "build");

	if(this->GroupName.compare("root")==0)
	{
		//bypass root device here; (now, "node-root" is FALSE as default)
		char const *name = strs[0].c_str();
		this->pNext = pNetChildrenList(1);
		this->pNext[0][0] = \
				new NetRootTree(this->doc, this->topology[name], this->physical[name], this->layer + 1, name);
	}
	else
	{
		expand_children(strs);
	}
	findMemberName(&this->topology, "inter", strs); //TODO:ass inter link
	findMemberName(&this->topology, "outer", strs); //TODO:add outer link callback
}

/* 1. Iterate (NetRootTree *next) Here */
void NetRootTree::expand_children(StringVector &Children)
{
	//"node-number" is Number; "node-config" is Array
	assert(this->physical["node-number"].IsInt());
	assert(this->physical["node-config"].IsArray());

	StringVector strs;
	NodesTuple tuple = {.id=0}; //TODO:*id* currently not used
	auto nNodes = this->physical["node-number"].GetInt();
	auto config = this->physical["node-config"].GetArray();

	/* Create Nodes Hierarchical */
	tuple.nodes.Create(nNodes);
	this->pNext = pNetChildrenList(nNodes);

	if(config.Empty())
	{
		HierPrint("End Of File", "default");
	}
	else
	{
		expand_config(config, Children);
	}
	
	/* Create Network Devices */
	findMemberName(&this->topology, "intra", strs);
	//TODO:"intra" links establish
}

/* 2. Expand Specified Config Here */
void NetRootTree::expand_config(Value::Array &config, StringVector &Children)
{
	int __start=0, __end=0;
	//FIXME:pNext SHOULD BE A Vector<Vector<NetRootTree *>>; rewrite iteration
	for(Value& v : config)
	{//iterate node
		//"node-index", "relative"/["index", "update", "append"]
		if(v["node-index"].IsInt())
		{
			__start = v["node-index"].GetInt();
			__end = __start;
		}
		else if(v["node-index"].IsObject())
		{
			__start = v["node-index"]["begin"].GetInt();
			__end = v["node-index"]["end"].GetInt();
		}
		sprintf(build_log, "Config[%d, %d]", __start, __end);
		HierPrint(build_log, "default");

		if(v.HasMember("relative"))
		{
			int index = v["relative"]["index"].GetInt();
			expand_template(config[index], v);
		}

		//remove useless item to assemble *config*
		v.RemoveMember("node-index"); //remove for children's usage
		for(int k=__start; k<=__end; ++k)
		{
			for(auto it = Children.begin(); it<Children.end(); ++it)
			{
				char const *child_name = it->c_str();
				assert(v[child_name].IsObject());
				this->pNext[k].push_back(new NetRootTree(this->doc, this->topology[child_name], v, this->layer + 1, child_name));
			}
		}
	}
}

/* Expand Template Here */
void NetRootTree::expand_template(Value &ref, Value &tmpl)
{
	char path[255];
	Value *val;
	Document tmp;
	auto &allocator = this->doc->GetAllocator();
	//Deep Copy to copied DOM
	tmp.CopyFrom(ref, allocator);

	/* Update operation on temp DOM */
	if(tmpl["relative"].HasMember("update"))
	{
		assert(tmpl["relative"]["update"].IsObject());
		for(auto& m : tmpl["relative"]["update"].GetObject())
		{
			strcpy(path, m.name.GetString());
			val = GetValueByPointer(tmp, path);
			*val = m.value.Move();
		}
	}

	/* Append operation on temp DOM array */
	if(tmpl["relative"].HasMember("append"))
	{
		assert(tmpl["relative"]["append"].IsObject());
		for(auto& m : tmpl["relative"]["append"].GetObject())
		{
			strcpy(path, m.name.GetString());
			val = GetValueByPointer(tmp, path);
			assert(val->IsArray()); //ensure is an Array
			// cout << val->GetArray().Size() << endl;
			val->PushBack(m.value, allocator);
			// cout << val->GetArray().Size() << endl;
		}
	}
	// Deep Copy and put back
	tmpl.CopyFrom(tmp, allocator);
}

int NetRootTree::getLayer() const
{
	return this->layer;
}

string NetRootTree::getName() const
{
	return this->GroupName;
}

void NetRootTree::getNextByIndex(const int num, pNetChildren &children)
{
	children = this->pNext[num];
}

void NetRootTree::getByGroupName(char const *name, pNetChildren &children)
{
	//TODO:join same GroupName under different config(children)
}

void NetRootTree::printLayers()
{
	printDocument("topology", &(this->topology));
	printDocument("physical", &(this->physical));
}

void NetRootTree::HierPrint(char const *str, string const &type="default")
{
	string m_indent(this->layer * 4, ' ');
	switch(PrintHelper[type])
	{
	case 0:
		printf("%sBuilding <%s>: \n", m_indent.c_str(), str);	
		break;
	case 1:
		printf("%s%s\n", m_indent.c_str(), str);
		break;
	default:
		printf("%s%s%s\n", m_indent.c_str(), m_indent.c_str(), str);
		break;
	}
}