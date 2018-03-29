
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
		/* delete this->doc; //FIXME: need to figure out why double free here. */
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
	StringVector tmp;

	HierPrint(this->GroupName.c_str(), "build");
	/* Iterate (NetRootTree *next) Here */
	findMemberName(&this->topology, "node", tmp);
	for(auto it = tmp.begin(); it<tmp.end(); ++it)
	{//iterate node group
		char const *name = move(it->c_str());
		NodesTuple tuple = {.id=0}; //FIXME:*id* currently not used

		//"node-number" is Number; "node-config" is Array
		assert(this->topology[name].IsObject());
		assert(this->physical[name].IsObject());
		auto nNodes = this->physical[name]["node-number"].GetInt();
		auto config = this->physical[name]["node-config"].GetArray();
		/* Create Nodes Hierarchical */
		tuple.nodes.Create(nNodes);
		this->pNext = move(pNetChildren(nNodes));
		for(Value& v : config)
		{//iterate node
			//"node-index", "relative"/["index", "update", "append"]
			int __start, __end;

			if(v["node-index"].IsInt())
			{
				__start = v["node-index"].GetInt();
				__end = __start;
			}
			if(v["node-index"].IsObject())
			{
				__start = v["node-index"]["begin"].GetInt();
				__end = v["node-index"]["end"].GetInt();
			}
			sprintf(build_log, "Config[%d, %d]", __start, __end);
			HierPrint(build_log, "default");

			// printf("(%d, %d)\n", __start, __end);
			if(v.HasMember("relative"))
			{
				int index = v["relative"]["index"].GetInt();
				expand_template(config[index], v);
			}

			//remove useless item to assemble *config*
			v.RemoveMember("node-index");
			for(int k=__start; k<=__end; ++k)
			{//iterate certain apply
				this->pNext[k] = \
					&NetRootTree(this->doc, this->topology[name], v, this->layer + 1, name);
			}
		}
		/* Create Network Devices */
		findMemberName(&this->topology, "intra", tmp);
	}
	findMemberName(&this->topology, "inter", tmp);
	findMemberName(&this->topology, "outer", tmp); //TODO:add outer link callback
}

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

NetRootTree const *NetRootTree::getNextByIndex(const int num) const
{
	return this->pNext[num];
}

NetRootTree const *NetRootTree::getNextByName(char const *name) const
{
	if(strcmp(name, this->GroupName.c_str())==0)
	{
		return this;
	}

	for(auto it=this->pNext.begin(); it!=this->pNext.end(); it++)
	{
		if((*it)->getName().compare(name)==0)
		{
			return *it;
		}
	}
	return NULL;
}

const NetRootTree *NetRootTree::getByGroupName(char const *name) const
{
	for(auto it=this->pNext.begin(); it!=this->pNext.end(); it++)
	{
		auto ptr = (*it)->getByGroupName(name);
		if(ptr!=NULL)
		{
			return ptr;
		}
	}
	return NULL;
}

void NetRootTree::printLayers()
{
	printDocument("topology", &(this->topology));
	printDocument("physical", &(this->physical));
}

void NetRootTree::HierPrint(char const *str, string const &type="default")
{
	string m_indent(this->layer, '\t');
	switch(PrintHelper[type])
	{
	case 0:
		printf("%sBuilding <%s>: \n", m_indent.c_str(), str);	
		break;
	case 1:
		printf("%s%s\n", m_indent.c_str(), str);
		break;
	default:
		printf("\t%s%s\n", m_indent.c_str(), str);
		break;
	}
}