#ifndef _HIS_DCOPCODE_H
#define _HIS_DCOPCODE_H

enum copcodes {
	DCMSG_NULL_ACTION = 0x0000,
	
	DCMSG_NEW_ENEITY,            /// ���󴴽�ʵ��
	DCMSG_LOAD_ENTITY,           /// ��ȡʵ��������ڵ�����������ڴ�ʵ����������ݿ��м���
	DCMSG_FLUSH_ENTITY,          /// ֪ͨ����ڵ�ʵ���Ѿ�����Ҫ��ʹ����
	DCMSG_QUERY_ENTITY,          /// ��ѯʵ��������ڵ�����������ڴ�ʵ�����򷵻ؿ�
	DCMSG_UPDATE_ENTITY,         /// ����ʵ��
	DCMSG_REMOVE_ENTITY,         /// ����ʵ�壬 ���ʵ�������ݿ���������ݿ���ɾ��
	DCMSG_LOAD_ENTITY_ONCE,		 //  ֻ����ʵ�壬�������ݳ�

	DCMSG_NEW_ENEITY_WITH_FLUSH,	// ����ʵ�壬�����������ݳ�

	DCMSG_ONLY_REMOVE_ENTITY,		// ����ɾ��cacheʵ�壬�����´��ټ��ص�ʱ�򣬴����ݿ����

	DCMSG_REMOVE_NET_MSG_QUEUE,  ///CacheSerializer::on_event head opcode
	DCMSG_INIT_SERIALIZE_FILE,   ///CacheSerializer::on_event head opcode�������µ�serialize�ļ�

	DCMSG_BINARY_FILE = 0x30,
	DCMSG_BINARY_RECORD,
};


#endif