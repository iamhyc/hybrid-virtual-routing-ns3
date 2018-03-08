
#include "ns3_net.h"

using namespace ns3_net;
using namespace rapidjson;

bool documentLint(rapidjson::Document const *json)
{
	//UNUSED(json);
	return true;
}

void printDocument(char const *name="", Value const *doc=0, int layer=0)
{
	string m_indent(layer, '\t');
	if (name != "")
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

NetRootTree::NetRootTree(rapidjson::Document const *json)
{

}

NetRootTree::~NetRootTree();

NetRootTree::string getName()
{
	return this.GroupName;
}
NetRootTree::NetRootTree *getNext()
{
	return this.next;
}

static const *NetRootTree NetRootTree::getByGroupName(const *NetRootTree root, const char *name)
{
	const NetRootTree *iter = root;
	while(root != NULL)
	{
		if (root.getByGroupName().compare(name))
			return iter;
		else
			iter = root->getNext();
	}
}
