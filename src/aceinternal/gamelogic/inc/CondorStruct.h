#ifndef __CONDOR_STRUCT_H_
#define __CONDOR_STRUCT_H_

#include <vector>
#pragma warning(push, 2)
#include "typedef.h"
#pragma warning(pop)

struct CondorWeapon {
    uint32 id;
    std::vector<uint32> rates;
    uint32 vip;
    uint32 zid;
    uint32 value;
    uint32 correct;
};

struct CondorPrey {
    uint32 id;
    std::vector<uint32> zids;
    std::vector<uint32> pas;
    std::vector<uint32> pbs;
    uint32 score;
};

struct CondorFormula {

	CondorFormula()
		:original_rounds(0)
		,times_per_round(0)
		,resource_type(0)
		,resource_num(0)
	{}
   
	uint32 original_rounds;
	uint32 times_per_round;
	uint32 resource_type;
	uint32 resource_num; 	
};

#endif
