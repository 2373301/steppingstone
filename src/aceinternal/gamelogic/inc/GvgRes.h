#ifndef GVG__RES_HPP
#define GVG__RES_HPP
struct GvgInfo
{
	GvgInfo()
	:exist(0)
	,kill(0)
	,level(0)
	,lifenum(0)
	,revivetime(0)
	,cdmin(0)
	,cdmax(0)
	,cdlast(0)
	{}
Uint32Vec_t begin1;
Uint32Vec_t begin2;
Uint32Vec_t begin3;
Uint32Vec_t begin4;
Uint32Vec_t end1;
Uint32Vec_t end2;
Uint32Vec_t end3;
Uint32Vec_t end4;
Uint32Vec_t enroll1;
Uint32Vec_t enroll2;
	uint32 exist;
	uint32 kill;
	uint32 level;
	uint32 lifenum;
Uint32Vec_t num1;
Uint32Vec_t num2;
Uint32Vec_t num3;
Uint32Vec_t num4;
	uint32 revivetime;
Uint32Vec_t rund1;
Uint32Vec_t rund2;
Uint32Vec_t rund3;
Uint32Vec_t rund4;
Uint32Vec_t sceneid;
uint32 cdmin;
uint32 cdmax;
uint32 cdlast;
};
#endif
