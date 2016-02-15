#ifndef  TRAINING_RES_HTTP
#define  TRAINING_RES_HTTP


struct TrainingPartnersInfo
{	
	TrainingPartnersInfo()
		:level_max(0)
	{

	}

	uint32 level_max;
	//>>失败,返还的安慰剂
	Uint32Vec_t placebo;
	//>>返还的安慰剂数量
	Uint32Vec_t placebo_num;

};

struct TrainingLevelTable
{
	TrainingLevelTable()
		:phy_v(0)
		,agi_v(0)
		,int_v(0)
		,phy_att(0)
		,agi_att(0)
		,int_att(0)
		,phy_def(0)
		,agi_def(0)
		,int_def(0)
		,hp(0)
		,rate(0)
		,drop(0)
		,cost(0)
		,group(0)
		,need_item(0)
		,need_num(0)
        ,first_strike(0)
	{

	}

	uint32 phy_v;
	uint32 agi_v;
	uint32 int_v;

	uint32 phy_att;
	uint32 agi_att;
	uint32 int_att;

	uint32 phy_def;
	uint32 agi_def;
	uint32 int_def;

	uint32 hp;

	uint32 rate;
	uint32 drop;
	uint32 cost;

	uint32 group;
	uint32 need_item;
	uint32 need_num;

    uint32 first_strike;
};

#endif