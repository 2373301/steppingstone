//
//#ifndef MANAGE_GRID_IMP_HPP
//#define MANAGE_GRID_IMP_HPP
//
//#include "Scene.h"
//
//class ManageGridImp : public ManageGrid
//{
//public:
//	ManageGridImp();
//
//	int init(SceneCfg * scene_cfg, Scene * scene, Pool * pool, vector<int> & map_id_vec);
//public:
//	virtual void init(vector<std::pair<int, int> > & map_ids);
//
//	virtual void add(typed::entity::Player * player);
//
//	virtual void readd(typed::entity::Player * player);
//
//	virtual void remove(typed::entity::Player * player);
//
//	virtual void search(EntityType entity_type, MapEntity & map_entity, GuidSet_t & guids);
//
//	virtual void search(EntityType entity_type, int map_id, GuidSet_t & guids);
//
//protected:
//	bool isNotifyOther(int map_type);
//
//	bool fillCreateOther(typed::entity::Player * player, typed::protocol::smsg_create_other * other_create);
//
//private:
//	typedef define_unordered_map<int, MapEntity> MapEntityMap_t;
//
//	SceneCfg * m_scene_cfg;
//
//	Scene * m_scene;
//
//	Pool * m_pool;
//
//	MapEntityMap_t	m_map_entity;
//};
//
//#endif