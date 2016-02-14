#ifndef _HIS_DCOPCODE_H
#define _HIS_DCOPCODE_H

enum copcodes {
	DCMSG_NULL_ACTION = 0x0000,
	
	DCMSG_NEW_ENEITY,            /// 请求创建实体
	DCMSG_LOAD_ENTITY,           /// 获取实例，缓存节点上如果不存在此实例，则从数据库中加载
	DCMSG_FLUSH_ENTITY,          /// 通知缓存节点实体已经不需要被使用了
	DCMSG_QUERY_ENTITY,          /// 查询实例，缓存节点上如果不存在此实例，则返回空
	DCMSG_UPDATE_ENTITY,         /// 更新实体
	DCMSG_REMOVE_ENTITY,         /// 销毁实体， 如果实体在数据库中则从数据库中删除
	DCMSG_LOAD_ENTITY_ONCE,		 //  只加载实体，不进数据池

	DCMSG_NEW_ENEITY_WITH_FLUSH,	// 创建实体，但不进入数据池

	DCMSG_ONLY_REMOVE_ENTITY,		// 仅仅删除cache实体，这样下次再加载的时候，从数据库加载

	DCMSG_REMOVE_NET_MSG_QUEUE,  ///CacheSerializer::on_event head opcode
	DCMSG_INIT_SERIALIZE_FILE,   ///CacheSerializer::on_event head opcode，创建新的serialize文件

	DCMSG_BINARY_FILE = 0x30,
	DCMSG_BINARY_RECORD,
};


#endif