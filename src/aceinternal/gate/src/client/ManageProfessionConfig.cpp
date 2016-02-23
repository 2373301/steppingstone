
#include "ManageProfessionConfig.h"
#include "ManageConfig.h"

ManageProfessionConfig::ManageProfessionConfig()
{

}

ManageProfessionConfig::~ManageProfessionConfig()
{

}

int ManageProfessionConfig::init()
{
	int result = 0;
	string config_file;
	//config_file = ManageConfig::instance()->getClientCfg().profession_config;
	if (loadConfig(config_file))
	{
		result = 0;
	}
	else
	{
		result = -1;
	}
	return result;
}

bool ManageProfessionConfig::hasProfessionType(int profession_type)
{
	ProfessionSpellMap::iterator it = m_profession_spell_map.find(profession_type);
	if (it != m_profession_spell_map.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

const ManageProfessionConfig::ProfessionSpell & ManageProfessionConfig::getProfessionSpell(int profession_type)
{
	return m_profession_spell_map[profession_type];
}

const Role_Skill_Template & ManageProfessionConfig::getProfessionSkillTemplate(int profession_type)
{
	return m_role_skill_template_map[profession_type];
}

void ManageProfessionConfig::initProfessionAttr(int32 profess_type, int32 * attr_array, int32 array_size)
{
	ProfessionInitAttrMap::iterator it = m_role_init_attr_map.find(profess_type);
	if (it != m_role_init_attr_map.end())
	{
		for (std::list<Role_Attr>::iterator it_attr = it->second.begin(); it_attr != it->second.end(); ++it_attr)
		{
			if (it_attr->index < array_size)
			{
				attr_array[it_attr->index] = it_attr->value;
			}
		}
	}
}

bool ManageProfessionConfig::loadConfig(const string & config_file)
{
	bool result = false;
	std::auto_ptr<Document> auto_doc(XMLFactory::create_document());
	if (NULL == auto_doc.get())
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to call XMLFactory::create_document()\n"));
		return false;
	}

//	try
	{
		if (auto_doc->load_file(config_file))
		{
			result = true;
			Element * root_ele = auto_doc->get_root();

			result = loadProfession(root_ele) && result;
			if (result)
			{
				initSkillTemplate();
			}
		}
		else
		{
			result = false;
			GATE_LOG_ERROR(ACE_TEXT("Failed to parse file : <%s>\n"), config_file.c_str());
		}
	}
	//catch (...)
	//{
	//	GATE_LOG_ERROR(ACE_TEXT("Raise unknown exception in ManageConfig::loadConfig, the last error is <%d>\n"), ACE_OS::last_error());
	//	result = false;
	//}
	return result;
}

bool ManageProfessionConfig::loadProfession(Element * root_element)
{
	bool result = true;
	if (NULL == root_element)
	{
		GATE_LOG_ERROR(ACE_TEXT("The root element is NULL\n"));
		return false;
	}
	Elements profession_list = root_element->get_elements("class");
	for (Elements::iterator it = profession_list.begin(); it != profession_list.end(); ++it)
	{
		Element * profession_ele = *it;
		if (NULL == profession_ele)
		{
			GATE_LOG_ERROR(ACE_TEXT("Failed to get profession element\n"));
			return false;
		}
		Attribute * attr_type = profession_ele->get_attribute("type");
		if (NULL == attr_type)
		{
			GATE_LOG_ERROR(ACE_TEXT("Failed to get attribute: type of profession\n"));
			return false;
		}
		int profession_type = attr_type->as_int();
		result = loadProfessionSpell(profession_type, profession_ele) && result;
		result = loadProfessionAttr(profession_type, profession_ele) && result;
	}
	return result;
}

bool ManageProfessionConfig::loadProfessionSpell(int profession_type, Element * profession_element)
{
	Element * skills = profession_element->get_element("skills");
	if (NULL == skills)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to get skills element\n"));
		return false;
	}
	Elements spell_list = skills->get_elements("spell");
	for (Elements::iterator it = spell_list.begin(); it != spell_list.end(); ++it)
	{
		Attribute * attr = (*it)->get_attribute("id");
		if (NULL != attr)
		{
			int chain = attr->as_int();
			m_profession_spell_map[profession_type].push_back(chain);
		}
		else
		{
			GATE_LOG_ERROR(ACE_TEXT("Failed to get spell attribute: chain\n"));
			return false;
		}
	}
	return true;
}

bool ManageProfessionConfig::loadProfessionAttr(int profession_type, Element * profession_element)
{
	Element * initiates = profession_element->get_element("initiates");
	if (NULL == initiates)
	{
		GATE_LOG_ERROR(ACE_TEXT("Failed to get initiates element\n"));
		// todo
		return true;
	}
	Elements initiate_list = initiates->get_elements("initiate");
	for (Elements::iterator it = initiate_list.begin(); it != initiate_list.end(); ++it)
	{
		int32 attr_index = 0;
		int32 attr_value = 0;
		Attributes attrs = (*it)->get_attributes();
		for (Attributes::iterator it_attr = attrs.begin(); it_attr != attrs.end(); ++it_attr)
		{
			if ((*it_attr)->get_name() == ACE_TEXT("att"))
			{
				attr_index = (*it_attr)->as_int();
			}
			else if ((*it_attr)->get_name() == ACE_TEXT("n"))
			{
				attr_value = (*it_attr)->as_int();
			}
		}
		m_role_init_attr_map[profession_type].push_back(Role_Attr(attr_index, attr_value));
	}
	return true;
}


void ManageProfessionConfig::initSkillTemplate()
{
	for (ProfessionSpellMap::iterator it = m_profession_spell_map.begin(); it != m_profession_spell_map.end(); ++it)
	{
		ProfessionSpell & profession_spell = it->second;
		int profession_type = it->first;

		uint32 * sk_guid = new uint32[profession_spell.size()];
		int64  * sk_cool = new int64[profession_spell.size()];
		uint32 * sk_level = new uint32[profession_spell.size()];
		uint64 * sk_release = new uint64[profession_spell.size()];
		int index = 0;
		for (ProfessionSpell::iterator it_sub = profession_spell.begin(); it_sub != profession_spell.end(); ++it_sub, ++index)
		{
			sk_guid[index] = *it_sub;
			sk_cool[index] = 0;
			sk_level[index] = 0;
			sk_release[index] = 0;
		}
		int32 array_size = profession_spell.size();
		m_role_skill_template_map[profession_type].skill_guid.resize(sizeof(uint32) + sizeof(uint32) * profession_spell.size());
		//memset((char *)m_role_skill_template_map[profession_type].skill_guid.data(), 0, sizeof(uint32) + sizeof(uint32) * profession_spell.size());
		memcpy((char *)m_role_skill_template_map[profession_type].skill_guid.data(), &array_size, sizeof(array_size));
		memcpy((char *)(m_role_skill_template_map[profession_type].skill_guid.data() + sizeof(array_size)), (char *)&sk_guid[0], sizeof(uint32) * profession_spell.size());

		m_role_skill_template_map[profession_type].skill_cool.resize(sizeof(int64) * profession_spell.size());
		memcpy((char *)m_role_skill_template_map[profession_type].skill_cool.data(), &array_size, sizeof(array_size));
		memcpy((char *)(m_role_skill_template_map[profession_type].skill_cool.data() + sizeof(array_size)), (char *)&sk_cool[0], sizeof(uint32) * profession_spell.size());
		// skill_level is same with skill_level
		m_role_skill_template_map[profession_type].skill_level.resize(sizeof(uint32) * profession_spell.size());
		memcpy((char *)m_role_skill_template_map[profession_type].skill_level.data(), &array_size, sizeof(array_size));
		memcpy((char *)(m_role_skill_template_map[profession_type].skill_level.data() + sizeof(array_size)), (char *)&sk_level[0], sizeof(uint32) * profession_spell.size());

		m_role_skill_template_map[profession_type].skill_release.resize(sizeof(uint64) * profession_spell.size());
		memcpy((char *)m_role_skill_template_map[profession_type].skill_release.data(), &array_size, sizeof(array_size));
		memcpy((char *)(m_role_skill_template_map[profession_type].skill_release.data() + sizeof(array_size)), (char *)&sk_release[0], sizeof(uint64) * profession_spell.size());
	}
}