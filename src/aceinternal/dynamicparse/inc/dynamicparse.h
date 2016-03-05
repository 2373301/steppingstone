#pragma once
#include "dp_def.h"
#include "typedef.h"
#include <vector>
#include <map>
#include <google/protobuf/dynamic_message.h>

class IDynamicParser;
class DYNAMIC_PARSE_EXPORT CacheAssistantx
{
public:
	CacheAssistantx();
	~CacheAssistantx();

	enum CACHE_ASSISTANT_ERROR
	{
		CAE_UNKNOWN = 0,
		CAE_NOT_FOUND,
		CAE_DUP_KEY,
		CAE_CANNOT_DEL,
		CAE_INTERNAL
	};

	google::protobuf::Message* data(void) { return msg; }
	bool load(void* query);
	bool update(void* query);
	bool insert(void* query);
	bool remove(void* query);
	size_t length(void) { return(data() ? data()->ByteSize() : 0); }
	CACHE_ASSISTANT_ERROR last_error(void) { return er_code_; }
	::std::string what(void) { return er_str_; }
	bool is_temp(void) { return is_temp_; }


	CACHE_ASSISTANT_ERROR er_code_ = CAE_UNKNOWN;
	::std::string er_str_;
	bool is_temp_ = false;
	friend class IDynamicParser;
	::google::protobuf::Message* msg = NULL;
	IDynamicParser *parser = NULL;
	uint64 myguid = 0;
};


struct StructValue;
typedef std::map<std::string, StructValue*> MessageMap;
typedef std::vector<StructValue *> MsgVec;

//ÿ��StructValue��ʾmessage���һ���ֶ�
typedef struct  StructValue
{
	StructValue()
		:type1(google::protobuf::FieldDescriptor::Label::LABEL_OPTIONAL)
		, type2(google::protobuf::FieldDescriptor::Type::TYPE_UINT32)
		, key_name("")
	{};

	~StructValue()
	{
	}

	google::protobuf::FieldDescriptor::Label	type1;		//�ֶ�����1�����磺optional��required��repeated
	google::protobuf::FieldDescriptor::Type		type2;		//�ֶ�����2�����磺TYPE_UINT32��
	std::string									key_name;	//�ֶ�����

	std::vector<std::string>					vec_value;	//�ֶ� type2 Ϊ non TYPE_MESSAGE ʱ��Ч
															//��� type1 Ϊ repeated ʱ�п����� N ��ֵ������ֻ�� 1 ��

	std::vector<MessageMap*>					vec_message;//�ֶ� type2 Ϊ TYPE_MESSAGEʱ ��Ч
															//��� type1 Ϊ repeated ʱ�п����� N ��ֵ������ֻ�� 1 ��
}StructValue;

typedef std::map<std::string, std::set<std::string> > TableColumnsMap;


class DYNAMIC_PARSE_EXPORT IDynamicParser
{
public:
	virtual int init(const std::string& proto_path) = 0;
	virtual int getMessageContent(const std::string& type_name, std::istream* input, MessageMap*& map_content) = 0;
	virtual void deleteMessageContent(MessageMap*& map_content) = 0;
	
	virtual CacheAssistantx * create(uint64 guid, const std::string& name) = 0;

	virtual int getMsgDesc(const std::string& type_name, MsgVec & vec) = 0;
	virtual void delMsgDesc(MsgVec & vec) = 0;

	virtual int querydbDesc(const std::string& a_ip, uint32 a_port, const std::string& a_user, const std::string a_pwd,
		const std::string& a_db_name, TableColumnsMap & a_outcols) = 0;
	virtual int checkEntity(const TableColumnsMap & a_outcols,
		const std::string& a_db_name, std::vector<std::string>& a_changed) = 0;
	virtual int updatedbChanged(const std::string& a_ip, uint32 a_port, const std::string& a_user, const std::string a_pwd,
		const std::string& a_db_name, std::vector<std::string>& a_changed) = 0;
};

DYNAMIC_PARSE_EXPORT IDynamicParser* createDynamicParser();