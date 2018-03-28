
#include "ns3_net.h"

using namespace std;
using namespace rapidjson;

const char* kTypeNames[] = 
	{ "Null", "False", "True", "Object", "Array", "String", "Number" };

void getNameBySplitter(char const *message, char const *splitter, vector<string> &tokens)
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

void printDocument(char const *name="", Value const *doc=0, int layer=0)
{
	string m_indent(layer, '\t');
	if (string(name).compare("")!=0)
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
				if (m.value[0].IsObject())
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

void findMemberName(Value const *doc, const std::string name, std::vector<std::string> &output)
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
		printf("__root_exit__\n");
		/* delete this->doc; //FIXME: need to figure out why double free here. */
	}
	else
	{
		printf("__%s_destruct__\n", this->GroupName.c_str());
	}
}

NetRootTree::NetRootTree(char const *path):
	GroupName("root")
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

NetRootTree::NetRootTree(Document *doc, Value &topo, Value &phy, char const *name) : GroupName(name)
{
	this->doc = doc;
	this->topology = topo;
	this->physical = phy;
	construct();
}

void NetRootTree::construct()
{
	printf("Building <%s>:\n", this->GroupName.c_str());
	/*iterate for (NetRootTree *next) here*/
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
}