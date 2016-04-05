
#ifndef MANAGE_ROLE_RES_HPP
#define MANAGE_ROLE_RES_HPP

#include "typedef.h"
#include "SimpleXML.h"
//#include "entity_ins.h"
#include "Singleton.h"
#include "SingletonInit.h"

struct RoleResInfo 
{
	RoleResInfo()
		: role_id(0)
		, level(0)
		, sex(0)
		, job(0)
		, char_ability(0)
		, str(0)
		, agi(0)
		, int_value(0)
		, phy_att(0)
		, phy_def(0)
		, agi_att(0)
		, agi_def(0)
		, int_att(0)
		, int_def(0)
		, hp(0)
		, hp_growth(0)
		, critical(0)
		, critical_damage(0)
		, block(0)
		, block_defence(0)
		, block_damage(0)
		, hit(0)
		, dodge(0)
		, normal_skill(0)
		, extremely_skill(0)
		, avatar_id(0)
		, hp_percent(0)
		, phy_att_percent(0)
		, phy_def_percent(0)
		, agi_att_percent(0)
		, agi_def_percent(0)
		, int_att_percent(0)
		, int_def_percent(0)
	{}

	uint32 role_id;
	int32 level;
	int32 sex;
	int32 job;
	int32 char_ability;
	int32 str;
	int32 agi;
	int32 int_value;
	int32 phy_att;
	int32 phy_def;
	int32 agi_att;
	int32 agi_def;
	int32 int_att;
	int32 int_def;
	int32 hp;
	int32 hp_growth;
	int32 critical;
	int32 critical_damage;
	int32 block;
	int32 block_defence;
	int32 block_damage;
	int32 hit;
	int32 dodge;
	int32 normal_skill;
	int32 extremely_skill;
	int32 avatar_id;
	int32 hp_percent;
	int32 phy_att_percent;
	int32 phy_def_percent;
	int32 agi_att_percent;
	int32 agi_def_percent;
	int32 int_att_percent;
	int32 int_def_percent;
};

typedef define_unordered_map<int, RoleResInfo *> RoleResInfoMap_t;

class ManageRoleRes : public SingletonInit
{
public:
	static ManageRoleRes * instance()
	{
		return Singleton<ManageRoleRes>::instance();
	}
public:
	virtual int init();
public:
	bool loadRoleFile(const string & role_file);

	const RoleResInfoMap_t & getRoleInfoMap();

	RoleResInfo * getRoleResInfo(int role_id);
protected:
	bool loadRoleInfo(Element * role_element);
private:
	RoleResInfoMap_t m_role_info_map;
};
#endif