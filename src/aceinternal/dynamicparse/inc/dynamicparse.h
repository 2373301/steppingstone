//dynamic_parse.h 
#ifndef _DYNAMIC_PARSE_H
#define _DYNAMIC_PARSE_H

#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>
#include <mysql++.h>
#include <vector>
#include <map>
#include "typedef.h"

class CDynamicParse;
class CacheAssistantx
{
public:
	CacheAssistantx(CDynamicParse* p);
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
	bool load(::mysqlpp::Query& query);
	bool update(::mysqlpp::Query& query);
	bool insert(::mysqlpp::Query& query);
	bool remove(::mysqlpp::Query& query);
	size_t length(void) { return(data() ? data()->ByteSize() : 0); }
	CACHE_ASSISTANT_ERROR last_error(void) { return er_code_; }
	::std::string what(void) { return er_str_; }
	bool is_temp(void) { return is_temp_; }

protected:
	CACHE_ASSISTANT_ERROR er_code_ = CAE_UNKNOWN;
	::std::string er_str_;
	bool is_temp_ = false;
	::google::protobuf::Message* msg = NULL;
	CDynamicParse *parser = NULL;
	uint64 myguid = 0;
};


struct StructValue;
typedef std::map<std::string, StructValue*> Map_Message;
typedef std::vector<StructValue *> MsgVec;

//每个StructValue表示message里的一个字段
typedef struct  StructValue
{
	StructValue()
		:type1(google::protobuf::FieldDescriptor::Label::LABEL_OPTIONAL)
		,type2(google::protobuf::FieldDescriptor::Type::TYPE_UINT32)
		,key_name("")
	{};

	~StructValue()
	{
	}

	google::protobuf::FieldDescriptor::Label	type1;		//字段类型1，例如：optional，required，repeated
	google::protobuf::FieldDescriptor::Type		type2;		//字段类型2，例如：TYPE_UINT32，
	std::string									key_name;	//字段名字

	std::vector<std::string>					vec_value;	//字段 type2 为 non TYPE_MESSAGE 时有效
															//如果 type1 为 repeated 时有可能有 N 个值，否则只有 1 个

	std::vector<Map_Message*>					vec_message;//字段 type2 为 TYPE_MESSAGE时 有效
															//如果 type1 为 repeated 时有可能有 N 个值，否则只有 1 个
}StructValue;

class CDynamicParse
{
public:
	CDynamicParse();
	~CDynamicParse();

public:
	/*
	/*功能：设置protobuf .proto文件所在文件夹，并读入文件里的类信息
	/*参数：proto_path[in]：proto文件所在文件夹，可以是相对路径或者绝对路径
	*/
	int init(const std::string& proto_path);
	
	/*
	/*功能：解析输入消息体的内容及相关信息
	/*参数：type_name[in]：		消息体名字，必须是全路径
	/*		input[in]：			message消息体二进制流
	/*		map_content[out]:	输出消息体内容
	*/
	int getMessageContent(const std::string& type_name, std::istream* input, Map_Message*& map_content);

	/*
	/*功能：删除存放消息内容的map资源
	/*参数：map_content[in, out]：待删除map指针，调用后该参数被赋值为NULL
	*/
	void deleteMessageContent(Map_Message*& map_content);


	// 查询第一层的结构
	int getMsgDesc(const std::string& type_name, MsgVec & vec);
	void delMsgDesc(MsgVec & vec);

	// 查询db中的table struct
	typedef std::map<std::string, std::set<std::string> > TableColumnsMap;
	int querydbDesc(const std::string& a_ip, uint32 a_port, const std::string& a_user, const std::string a_pwd,
					const std::string& a_db_name, TableColumnsMap & a_outcols);

	int checkEntity(const TableColumnsMap & a_outcols, const std::string& a_db_name, std::vector<std::string>& a_changed);

	int updatedbChanged(const std::string& a_ip, uint32 a_port, const std::string& a_user, const std::string a_pwd,
				const std::string& a_db_name, std::vector<std::string>& a_changed);

private:
	google::protobuf::Message* createMessage(const std::string& typeName);
	void ReadDesAndInsert(const google::protobuf::Message& message, Map_Message*& map_content);
	void deleteMsg(Map_Message*& map_content);
private:
	google::protobuf::compiler::Importer * m_importer;
	google::protobuf::DynamicMessageFactory* m_dynamic_message_factory;
	std::vector<std::string> m_protofiles;
};

#endif