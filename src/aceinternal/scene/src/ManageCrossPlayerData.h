
#ifndef MANAGE_CROSS_PLAYERDATA_HPP
#define MANAGE_CROSS_PLAYERDATA_HPP

#include "Scene.h"
#include "PoolImp.h"
#include "opcode.h"

struct CrossPlayerDataInfo 
{
	CrossPlayerDataInfo()
	: player_guid(0)
	, key("default")
	{}

	string platform;
	string line_id;
	uint64 player_guid;
	string key;
	typedef map<uint64, MSG_TYPE *> MsgMap_t;
	MsgMap_t msg_map;
};

class ManageCrossPlayerData
{
public:
	ManageCrossPlayerData();
	~ManageCrossPlayerData();
public:
	int init(const SceneCfg * scene_cfg);

	void collectCrossPlayerData(const string & line_id, const string & platform, uint64 player_guid, uint64 entity_guid, const string & entity_stream);

	void collectCrossPlayerDataFinish(const string & line_id, const string & platform, uint64 player_guid);

	void deliveryCrossPlayerDataToGS();

	void deliveryCrossPlayerDataToGS(const string & platform, const string & line_id);

//	void lookupCrossPlayerInfo(uint64 observer, uint64 player_guid);

	void getCrossPlayerDataInfo(uint64 player_guid, map<uint64, MSG_TYPE *> & msg_map);

protected:
	void initCrossPlayerData();

	void loadCrossPlayerData();

	void loadCrossPlayerData(const string & platform, const string & line_id, const string & line_path);

	string getKey(const string & line_id, const string & platform, uint64 player_guid);

	void saveData(CrossPlayerDataInfo * cross_player_data_info);

	void saveFile(const string & file_name, const string & file_stream);

	void createDir(const string & file_dir);

	void loadStream(const string & file_name, string & msg_stream);

	uint64 extractGuid(const string & file_name);

	void deliveryCrossPlayerDataToGS(CrossPlayerDataInfo * cross_pd_info);

	void deliveryCrossPlayerDataToGS(const string & platform, const string & line_id, CrossPlayerDataInfo * cross_pd_info);

private:
	SceneCfg * m_scene_cfg;

	PoolImp m_pool;

	typedef map<string, CrossPlayerDataInfo *> CrossPlayerDataInfoMap_t;

	CrossPlayerDataInfoMap_t m_cross_player_data_info_map;

	typedef map<uint64, CrossPlayerDataInfo *> CrossPlayerGuidDataInfoMap_t;

	CrossPlayerGuidDataInfoMap_t m_cross_player_guid_data_info_map;
};

#endif