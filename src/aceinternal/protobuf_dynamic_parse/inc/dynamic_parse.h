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

//每个StructValue表示message里的一个字段
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

	FieldDescriptor::Label	type1;			//字段类型1，例如：optional，required，repeated
	FieldDescriptor::Type	type2;			//字段类型2，例如：TYPE_UINT32，
	string					key_name;		//字段名字

	vector<string>			vec_value;		//字段 type2 为 non TYPE_MESSAGE 时有效
											//如果 type1 为 repeated 时有可能有 N 个值，否则只有 1 个

	vector<Map_Message*>	vec_message;	//字段 type2 为 TYPE_MESSAGE时 有效
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
	int init(const string& proto_path);
	
	/*
	/*功能：解析输入消息体的内容及相关信息
	/*参数：type_name[in]：		消息体名字，必须是全路径
	/*		input[in]：			message消息体二进制流
	/*		map_content[out]:	输出消息体内容
	*/
	int getMessageContent(const std::string& type_name, istream* input, Map_Message*& map_content);

	/*
	/*功能：删除存放消息内容的map资源
	/*参数：map_content[in, out]：待删除map指针，调用后该参数被赋值为NULL
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