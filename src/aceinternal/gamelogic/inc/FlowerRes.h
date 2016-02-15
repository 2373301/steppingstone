
#ifndef FLOWER_RES_HPP
#define FLOWER_RES_HPP

struct FlowerRes 
{
	FlowerRes()
	:id(0)
	, num(0)
	, reword_value(0)
	, type_value(0)
	, pay_value(0)	
	{}

	uint32 id;
	uint32 num;
	uint32 reword_value;
	uint32 type_value;
	uint32 pay_value;
};
#endif