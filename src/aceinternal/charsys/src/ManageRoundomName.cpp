
#include "ManageRoundomName.h"
#include "ManageConfig.h"

int ManageRoundomName::init()
{
	srand( (unsigned)time( NULL ) );

	return loadConfig(ManageConfig::instance()->getRandomNameCfg());
}

bool ManageRoundomName::loadConfig(const string & cfg_path)
{
	std::auto_ptr<Document> doc(XMLFactory::create_document());
	if (doc.get() == NULL)
	{
		return -1;
	}

	if (doc->load_file(cfg_path))
	{
		Element * root_ele = doc->get_root();
		return loadNameCfg(root_ele);
	}

	return false;
}

bool ManageRoundomName::loadNameCfg(Element * root_ele)
{
	if (NULL == root_ele)
	{
		return false;
	}

	string attr_name_name = "xing";
	string attr_female_name = "female";
	string attr_male_name = "male";

	Elements eles = root_ele->get_elements();
	for (Elements::iterator it = eles.begin(); it != eles.end(); ++it)
	{
		Attributes attrs = (*it)->get_attributes();
		for (Attributes::iterator attr_it = attrs.begin(); attr_it != attrs.end(); ++attr_it)
		{
			Attribute * attr = *attr_it;
			if (attr->get_name() == attr_name_name)
			{
				if (attr->get_value().length() > 0)
				{
					m_random_name_info.surname.push_back(attr->get_value());
				}
			}
			else if (attr->get_name() == attr_female_name)
			{
				if (attr->get_value().length() > 0)
				{
					m_random_name_info.female_name.push_back(attr->get_value());
				}
			}
			else if (attr->get_name() == attr_male_name)
			{
				if (attr->get_value().length() > 0)
				{
					m_random_name_info.male_name.push_back(attr->get_value());
				}
			}
		}
	}

	return true;
}

void ManageRoundomName::getNewName(int sex, string & new_name)
{
	m_random_name_info.getNewName(sex, new_name);
}