
#ifndef MANAGE_PLAYER_INIT_HPP
#define MANAGE_PLAYER_INIT_HPP

#include "Singleton.h"
#include "SingletonInit.h"
#include "typedef.h"
#include "SimpleXML.h"

typedef std::vector<uint64> Uint64Vec_t;
typedef std::vector<uint32> Uint32Vec_t;

struct InitializationCfg 
{
	InitializationCfg()
	: level(0)
	, exp(0)
	, money(0)
	, coin(0)
	, src_x(0)
	, src_y(0)
	, seniority(0)
	, coin_exp(0)
	, frame(0)
	, energy(0)
	, training_slot(0)
	, member_slot(0)
	, curr_battle_array(0)
	, map_id(0)
	{}

	int level;
	int exp;
	int money;
	int coin;
	int src_x;
	int src_y;
	int seniority;
	int coin_exp;
	int frame;
	int energy;
	int training_slot;
	int member_slot;
	int curr_battle_array;
	int map_id;
	Uint32Vec_t ba_role_location;
	Uint32Vec_t battle_array_list;
	Uint32Vec_t wonder_skill;
	Uint64Vec_t buffers;
	Uint64Vec_t roles;
	Uint64Vec_t bag;
	Uint64Vec_t warehouse;
	Uint32Vec_t can_recruit_roles;
};

class ManagePlayerInit : public SingletonInit
{
public:
	static ManagePlayerInit * instance()
	{
		return Singleton<ManagePlayerInit>::instance();
	}
public:
	virtual int init();

	InitializationCfg & getPlayerInitCfg();
protected:
	bool loadConfig(const string & cfg_path);

	bool loadInitCfg(Element * root_ele);

	bool getAttr(Element * element, const string & attr_name, int & attr_value);

	bool getAttr(Element * element, const string & attr_name, string & attr_value);

	bool getAttr(Element * element, const string & attr_name, Uint32Vec_t & attr_value);

	bool getAttr(Element * element, const string & attr_name, Uint64Vec_t & attr_value);
private:
	InitializationCfg m_init_cfg;
};

#endif