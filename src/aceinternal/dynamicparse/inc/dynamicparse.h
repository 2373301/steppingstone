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

//ÿ��StructValue��ʾmessage���һ���ֶ�
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

	google::protobuf::FieldDescriptor::Label	type1;		//�ֶ�����1�����磺optional��required��repeated
	google::protobuf::FieldDescriptor::Type		type2;		//�ֶ�����2�����磺TYPE_UINT32��
	std::string									key_name;	//�ֶ�����

	std::vector<std::string>					vec_value;	//�ֶ� type2 Ϊ non TYPE_MESSAGE ʱ��Ч
															//��� type1 Ϊ repeated ʱ�п����� N ��ֵ������ֻ�� 1 ��

	std::vector<Map_Message*>					vec_message;//�ֶ� type2 Ϊ TYPE_MESSAGEʱ ��Ч
															//��� type1 Ϊ repeated ʱ�п����� N ��ֵ������ֻ�� 1 ��
}StructValue;

class CDynamicParse
{
public:
	CDynamicParse();
	~CDynamicParse();

public:
	/*
	/*���ܣ�����protobuf .proto�ļ������ļ��У��������ļ��������Ϣ
	/*������proto_path[in]��proto�ļ������ļ��У����������·�����߾���·��
	*/
	int init(const std::string& proto_path);
	
	/*
	/*���ܣ�����������Ϣ������ݼ������Ϣ
	/*������type_name[in]��		��Ϣ�����֣�������ȫ·��
	/*		input[in]��			message��Ϣ���������
	/*		map_content[out]:	�����Ϣ������
	*/
	int getMessageContent(const std::string& type_name, std::istream* input, Map_Message*& map_content);

	/*
	/*���ܣ�ɾ�������Ϣ���ݵ�map��Դ
	/*������map_content[in, out]����ɾ��mapָ�룬���ú�ò�������ֵΪNULL
	*/
	void deleteMessageContent(Map_Message*& map_content);


	// ��ѯ��һ��Ľṹ
	int getMsgDesc(const std::string& type_name, MsgVec & vec);
	void delMsgDesc(MsgVec & vec);

	// ��ѯdb�е�table struct
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