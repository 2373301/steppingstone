#ifndef _MANAGE_PROFESSION_CONFIG_HPP
#define _MANAGE_PROFESSION_CONFIG_HPP

#include <map>
#include <vector>
#include <string>
#include "Singleton.h"
#include "SimpleXML.h"
#include "GateLogger.h"
#include "typedef.h"

using std::string;

struct Role_Skill_Template 
{
	string skill_guid;
	string skill_cool;
//	string skill_status;
	string skill_level;
	string skill_release;
};

struct Role_Attr 
{
	Role_Attr()
	: index(0)
	, value(0)
	{}
	Role_Attr(int32 ind, int32 val)
	: index(ind)
	, value(val)
	{}

	int32 index;
	int32 value;
};

class ManageProfessionConfig
{
public:
	ManageProfessionConfig();
	~ManageProfessionConfig();
public:
	static ManageProfessionConfig * instance()
	{
		return Singleton<ManageProfessionConfig>::instance();
	}
public:
	typedef std::vector<int> ProfessionSpell;

	typedef std::map<int, ProfessionSpell > ProfessionSpellMap;

	typedef std::map<int, Role_Skill_Template>  RoleSkillTemplateMap;

	typedef std::map<int, std::list<Role_Attr> > ProfessionInitAttrMap;

	int init();

	bool hasProfessionType(int profession_type);

	const ProfessionSpell & getProfessionSpell(int profession_type);

	const Role_Skill_Template & getProfessionSkillTemplate(int profession_type);

	void initProfessionAttr(int32 profess_type, int32 * attr_array, int32 array_size);
protected:
	bool loadConfig(const string & config_file);

	bool loadProfession(Element * root_element);

	bool loadProfessionSpell(int profession_type, Element * profession_element);

	bool loadProfessionAttr(int profession_type, Element * profession_element);

	void initSkillTemplate();
private:
	ProfessionSpellMap m_profession_spell_map;

	RoleSkillTemplateMap  m_role_skill_template_map;

	ProfessionInitAttrMap m_role_init_attr_map;
};

#endif