
#include "ns3_net.h"

using namespace rapidjson;
const char* kTypeNames[] = 
	{ "Null", "False", "True", "Object", "Array", "String", "Number" };

void getNameBySplitter(char const *message, char const *splitter, vector<string> &tokens)
{
	/*e.g. getNamebySplitter("ipBase", "_", tokens); ("device", "-", tokens)*/
	boost::split(tokens, message, boost::is_any_of(splitter));
}

bool documentLint(rapidjson::Document const &json)
{
	assert(json["topology"].IsObject());
	assert(json["physical"].IsObject());
	assert(json["application"].IsObject());
	return true;
}

void printDocument(char const *name="", Value const *doc=0, int layer=0)
{
	string m_indent(layer, '\t');
	if (string(name).compare(""))
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

void findMemberName(rapidjson::Value const *doc, const std::string name, std::vector<std::string> &output)
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
const char* kChannelNames[] = { "csma", "wifi", "p2p" };

NetRootTree::~NetRootTree()
{

}

NetRootTree::NetRootTree(char const *path)
{
	ifstream ifs(path, fstream::in);
	IStreamWrapper isw(ifs);
	rapidjson::Document json;

	auto flag = json.ParseStream(isw).HasParseError();
	assert(flag == 0);
	documentLint(json); //use assert

	Value* topology = GetValueByPointer(json, "/topology");
	Value* physical = GetValueByPointer(json, "/physical");
	Value* application = GetValueByPointer(json, "/application");

	/*iterate for (NetRootTree *next) here*/
	assert(topology->IsObject());
	assert(physical->IsObject());
	assert(application->IsObject());

	printDocument("topology", topology);
	printDocument("physical", physical);
	printDocument("application", application);

	// cout << topology->HasMember("device-router") << endl;
}

string NetRootTree::getName()
{
	return GroupName;
}

NetRootTree const *NetRootTree::getNext()
{
	return next;
}

/*static const NetRootTree *NetRootTree::getByGroupName(const NetRootTree *root, char const *name)
{
	const NetRootTree *iter = root;
	while(root != NULL)
	{
		if (root->getName().compare(name))
			return iter;
		else
			iter = root->getNext();
	}
}
*/