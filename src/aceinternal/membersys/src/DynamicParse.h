//dynamic_parse.h 
#ifndef _DYNAMIC_PARSE_H
#define _DYNAMIC_PARSE_H

//#include <google/protobuf/descriptor.h>
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>
#include <vector>
//#include <map>
#include <boost/unordered_map.hpp>
#include "Singleton.h"

using namespace google::protobuf;

struct StructValue;
typedef map<string, StructValue*> Map_Message;

//ÿ��StructValue��ʾmessage���һ���ֶ�
typedef struct StructValue
{
	StructValue()
		:type1(FieldDescriptor::LABEL_OPTIONAL)
		,type2(FieldDescriptor::TYPE_UINT32)
		,key_name("")
	{};

	~StructValue()
	{
	}

	FieldDescriptor::Label	type1;			//�ֶ�����1�����磺optional��required��repeated
	FieldDescriptor::Type	type2;			//�ֶ�����2�����磺TYPE_UINT32��
	string					key_name;		//�ֶ�����
	vector<string>			vec_value;		//���ֶ�type2Ϊ��TYPE_MESSAGEʱ��Ч�����type1Ϊrepeatedʱ�п����ж��ֵ������ֻ��һ��
	vector<Map_Message*>	vec_message;	//���ֶ�type2ΪTYPE_MESSAGEʱ��Ч�����type1Ϊrepeatedʱ�п����ж��ֵ������ֻ��һ��
};


class DynamicParse
{
public:
	DynamicParse();
	~DynamicParse();
public:
	static DynamicParse * instance()
	{
		return Singleton<DynamicParse>::instance();
	}

public:
	/*---------------------------
	/*
	/*���ܣ�����protobuf .proto�ļ������ļ��У��������ļ��������Ϣ
	/*
	/*������proto_path[in]��proto�ļ������ļ��У����������·�����߾���·��
	/*
	/--------------------------*/
	int init(const string& proto_path);
	
	/*---------------------------
	/*
	/*���ܣ�����������Ϣ������ݼ������Ϣ
	/*������type_name[in]��		��Ϣ�����֣�������ȫ·��
	/*		input[in]��			message��Ϣ���������
	/*		map_content[out]:	�����Ϣ������
	/--------------------------*/
	int getMessageContent(const std::string& type_name, istream* input, Map_Message*& map_content);

	int getMessageContent(google::protobuf::Message* msg, Map_Message*& map_content);

	/*---------------------------
	/*
	/*���ܣ�ɾ�������Ϣ���ݵ�map��Դ
	/*������map_content[in, out]����ɾ��mapָ�룬���ú�ò�������ֵΪNULL
	/*
	/--------------------------*/
	void deleteMessageContent(Map_Message*& map_content);

	void getJsonFormat(google::protobuf::Message * msg, string & json_format);

	void getMessageContent(Map_Message * map_content, string & json_format);

private:
	google::protobuf::Message* createMessage(const std::string& typeName);
	void ReadDesAndInsert(const google::protobuf::Message& message, Map_Message*& map_content);
	void deleteMsg(Map_Message*& map_content);
private:
	google::protobuf::compiler::Importer * m_importer;
	google::protobuf::DynamicMessageFactory* m_dynamic_message_factory;
};

#endif