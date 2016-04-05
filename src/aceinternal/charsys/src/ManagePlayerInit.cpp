
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "ManagePlayerInit.h"
#include "SimpleXML.h"
#include "ManageConfig.h"

int ManagePlayerInit::init()
{
	if (loadConfig(ManageConfig::instance()->getInitPlayerCfg()))
	{
		return 0;
	}
	return -1;
}

InitializationCfg & ManagePlayerInit::getPlayerInitCfg()
{
	return m_init_cfg;
}

bool ManagePlayerInit::loadConfig(const string & cfg_path)
{
	std::auto_ptr<Document> doc(XMLFactory::create_document());
	if (doc.get() == NULL)
	{
		return false;
	}

	if (doc->load_file(cfg_path))
	{
		Element * root_ele = doc->get_root();
		return loadInitCfg(root_ele);
	}

	return false;
}

bool ManagePlayerInit::loadInitCfg(Element * root_ele)
{
	if (NULL == root_ele)
	{
		return false;
	}

	Element * init_ele = root_ele->get_element("initialization");
	if (NULL == init_ele)
	{
		return NULL;
	}

	bool result = true;

	result = getAttr(init_ele, "level", m_init_cfg.level) && result;
	result = getAttr(init_ele, "exp", m_init_cfg.exp) && result;
	result = getAttr(init_ele, "money", m_init_cfg.money) && result;
	result = getAttr(init_ele, "coin", m_init_cfg.coin) && result;
	result = getAttr(init_ele, "src_x", m_init_cfg.src_x) && result;
	result = getAttr(init_ele, "src_y", m_init_cfg.src_y) && result;
	result = getAttr(init_ele, "seniority", m_init_cfg.seniority) && result;
	result = getAttr(init_ele, "coin_exp", m_init_cfg.coin_exp) && result;
	result = getAttr(init_ele, "frame", m_init_cfg.frame) && result;
	result = getAttr(init_ele, "energy", m_init_cfg.energy) && result;
	result = getAttr(init_ele, "training_slot", m_init_cfg.training_slot) && result;
	result = getAttr(init_ele, "member_slot", m_init_cfg.member_slot) && result;
	result = getAttr(init_ele, "curr_battle_array", m_init_cfg.curr_battle_array) && result;
	result = getAttr(init_ele, "map_id", m_init_cfg.map_id) && result;


	result = getAttr(init_ele, "ba_role_location", m_init_cfg.ba_role_location) && result;
	result = getAttr(init_ele, "battle_array_list", m_init_cfg.battle_array_list) && result;
	result = getAttr(init_ele, "wonder_skill", m_init_cfg.wonder_skill) && result;
	result = getAttr(init_ele, "buffers", m_init_cfg.buffers) && result;
	result = getAttr(init_ele, "roles", m_init_cfg.roles) && result;
	result = getAttr(init_ele, "bag", m_init_cfg.bag) && result;
	result = getAttr(init_ele, "warehouse", m_init_cfg.warehouse) && result;
	result = getAttr(init_ele, "can_recruit_roles", m_init_cfg.can_recruit_roles) && result;

	return result;
}

bool ManagePlayerInit::getAttr(Element * element, const string & attr_name, int & attr_value)
{
	Attribute * attr = element->get_attribute(attr_name);
	if (NULL ==  attr)
	{
		return false;
	}

	attr_value = attr->as_int();
	return true;
}

bool ManagePlayerInit::getAttr(Element * element, const string & attr_name, string & attr_value)
{
	Attribute * attr = element->get_attribute(attr_name);
	if (NULL ==  attr)
	{
		return false;
	}

	attr_value = attr->as_cstr();
	return true;
}

bool ManagePlayerInit::getAttr(Element * element, const string & attr_name, Uint32Vec_t & attr_value)
{
	Attribute * attr = element->get_attribute(attr_name);
	if (NULL ==  attr)
	{
		return false;
	}

	string av = attr->as_cstr();
	vector<string> str_vec;
	boost::split(str_vec, av, boost::is_any_of(";"));
	int v = 0;

	for (vector<string>::iterator it = str_vec.begin(); it != str_vec.end(); ++it)
	{
		v = 0;

		try
		{
			v = boost::lexical_cast<int>(*it);
		}
		catch (boost::bad_lexical_cast * e)
		{
			return false;
		}

		attr_value.push_back(v);
	}
	return true;
}

bool ManagePlayerInit::getAttr(Element * element, const string & attr_name, Uint64Vec_t & attr_value)
{
	Attribute * attr = element->get_attribute(attr_name);
	if (NULL ==  attr)
	{
		return false;
	}

	string av = attr->as_cstr();
	vector<string> str_vec;
	boost::algorithm::split(str_vec, av, boost::is_any_of(";"));
	uint64 v = 0;
	for (vector<string>::iterator it = str_vec.begin(); it != str_vec.end(); ++it)
	{
		v = 0;
		try
		{
			v = boost::lexical_cast<uint64>(*it);
		}
		catch (boost::bad_lexical_cast * e)
		{
			return false;
		}

		attr_value.push_back(v);
	}
	return true;
}