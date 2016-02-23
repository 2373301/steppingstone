
#ifndef MANAGE_REDUPLICATION_HPP
#define MANAGE_REDUPLICATION_HPP

#include "SimpleXML.h"
#include "Singleton.h"
#include "typedef.h"

struct RedulicationInfo 
{
	RedulicationInfo()
	{}

	uint32	map_id;
	uint32  target_map_id;
	int		src_x;
	int		src_y;
};

class ManageReduplication
{
public:
	static ManageReduplication * instance()
	{
		return Singleton<ManageReduplication>::instance();
	}
public:
	int init();

	uint32 getMapId(uint32 map_id);
protected:
private:
	typedef define_unordered_map<uint32, RedulicationInfo *> MapInfoMap_t;

	MapInfoMap_t m_map_info_map; //副本跳图到正常地图
};
#endif