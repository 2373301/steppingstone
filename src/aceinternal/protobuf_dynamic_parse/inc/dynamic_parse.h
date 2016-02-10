//dynamic_parse.h 
#ifndef _DYNAMIC_PARSE_H
#define _DYNAMIC_PARSE_H

#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>
#include <vector>
#include <map>

using namespace google::protobuf;

struct StructValue;
typedef std::map<string, StructValue*> Map_Message;
typedef std::vector<StructValue *> MsgVec;

//ÿ��StructValue��ʾmessage���һ���ֶ�
typedef struct  StructValue
{
	StructValue()
		:type1(FieldDescriptor::Label::LABEL_OPTIONAL)
		,type2(FieldDescriptor::Type::TYPE_UINT32)
		,key_name("")
	{};

	~StructValue()
	{
	}

	FieldDescriptor::Label	type1;			//�ֶ�����1�����磺optional��required��repeated
	FieldDescriptor::Type	type2;			//�ֶ�����2�����磺TYPE_UINT32��
	string					key_name;		//�ֶ�����

	vector<string>			vec_value;		//�ֶ� type2 Ϊ non TYPE_MESSAGE ʱ��Ч
											//��� type1 Ϊ repeated ʱ�п����� N ��ֵ������ֻ�� 1 ��

	vector<Map_Message*>	vec_message;	//�ֶ� type2 Ϊ TYPE_MESSAGEʱ ��Ч
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
	int init(const string& proto_path);
	
	/*
	/*���ܣ�����������Ϣ������ݼ������Ϣ
	/*������type_name[in]��		��Ϣ�����֣�������ȫ·��
	/*		input[in]��			message��Ϣ���������
	/*		map_content[out]:	�����Ϣ������
	*/
	int getMessageContent(const std::string& type_name, istream* input, Map_Message*& map_content);

	/*
	/*���ܣ�ɾ�������Ϣ���ݵ�map��Դ
	/*������map_content[in, out]����ɾ��mapָ�룬���ú�ò�������ֵΪNULL
	*/
	void deleteMessageContent(Map_Message*& map_content);



	int getMsgDesc(const std::string& type_name, MsgVec & vec);
	void delMsgDesc(MsgVec & vec);

private:
	google::protobuf::Message* createMessage(const std::string& typeName);
	void ReadDesAndInsert(const google::protobuf::Message& message, Map_Message*& map_content);
	void deleteMsg(Map_Message*& map_content);
private:
	google::protobuf::compiler::Importer * m_importer;
	google::protobuf::DynamicMessageFactory* m_dynamic_message_factory;
};

#endif