//dynamic_parse.cpp
#include "dynamic_parse.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>


using namespace google::protobuf;


class PError : public google::protobuf::compiler::MultiFileErrorCollector
{
	virtual void AddError(const string& filename, int line, int column,
		const string& message)
	{}
};


CDynamicParse::CDynamicParse()
{

}

CDynamicParse::~CDynamicParse()
{
	if (m_dynamic_message_factory)
	{
		delete m_dynamic_message_factory;
	}
	if (m_importer)
	{
		delete m_importer;
	}
}

int CDynamicParse::init(const string& proto_path)
{
	m_dynamic_message_factory = new DynamicMessageFactory();
	if (m_dynamic_message_factory == NULL)
	{
		return false;
	}

	PError errorCollector;
	google::protobuf::compiler::DiskSourceTree sourceTree;
	m_importer = new google::protobuf::compiler::Importer(&sourceTree, &errorCollector); 
	sourceTree.MapPath("", proto_path); 


	boost::filesystem::path fsPath(proto_path);
	boost::filesystem::directory_iterator beg_iter(fsPath);
	boost::filesystem::directory_iterator end_iter;
	for (; beg_iter != end_iter; ++beg_iter)
	{
		if (boost::filesystem::is_directory(*beg_iter))
		{
			continue;
		}
		else
		{
			string str_name = beg_iter->path().filename().generic_string();
			m_importer->Import(str_name);
		}
	}

	return true;
}


void CDynamicParse::ReadDesAndInsert(const google::protobuf::Message& message, 	Map_Message*& map_content)
{
	map_content = new Map_Message;

	const google::protobuf::Descriptor* des = message.GetDescriptor();
	const google::protobuf::Reflection* refl = message.GetReflection();
	int count = des->field_count();
	for (int i=0; i<count; ++i)
	{
		const google::protobuf::FieldDescriptor* field_des = des->field(i);
		string str_name = field_des->name();


		StructValue* str_value = new StructValue;
		str_value->type1 = field_des->label();
		str_value->type2 = field_des->type();
		str_value->key_name = str_name;

		map_content->insert(make_pair(str_name, str_value));

		//repeated
		if (field_des->is_repeated())
		{
			int field_size = refl->FieldSize(message, field_des);
			for (int j=0; j<field_size; ++j)
			{	
				if (field_des->type() == FieldDescriptor::TYPE_STRING 
					|| field_des->type() == FieldDescriptor::TYPE_BYTES)
				{
					str_value->vec_value.push_back(refl->GetRepeatedString(message, field_des, j));
				}
				else if (field_des->type() == FieldDescriptor::TYPE_MESSAGE)
				{
					const google::protobuf::Message& msg_sub = refl->GetRepeatedMessage(message, field_des, j);
					Map_Message* map_msg;
					ReadDesAndInsert(msg_sub, map_msg);
					str_value->vec_message.push_back(map_msg);
				}
				else
				{
					char value[128] = {0};
					switch (field_des->type())
					{
					case FieldDescriptor::TYPE_INT32:
						_itoa(refl->GetRepeatedInt32(message, field_des, j), value, 10);
						break;
					case FieldDescriptor::TYPE_UINT32:
						_itoa(refl->GetRepeatedUInt32(message, field_des, j), value, 10);
						break;
					case FieldDescriptor::TYPE_INT64:
						_i64toa(refl->GetRepeatedInt64(message, field_des, j), value, 10);
						break;
					case FieldDescriptor::TYPE_UINT64:
						_ui64toa(refl->GetRepeatedUInt64(message, field_des, j), value, 10);
						break;
					default:
						break;
					}
					str_value->vec_value.push_back(string(value));
				}
			}
		}
		else
		{
			if (field_des->type() == FieldDescriptor::TYPE_STRING 
				|| field_des->type() == FieldDescriptor::TYPE_BYTES)
			{
				str_value->vec_value.push_back(refl->GetString(message, field_des));
			}
			else if (field_des->type() == FieldDescriptor::TYPE_MESSAGE)
			{
				const google::protobuf::Message& msg_sub = refl->GetMessage(message, field_des);
				Map_Message* map_msg;
				ReadDesAndInsert(msg_sub, map_msg);
				str_value->vec_message.push_back(map_msg);
			}
			else
			{
				char value[128] = {0};
				switch (field_des->type())
				{
				case FieldDescriptor::TYPE_INT32:
					_itoa(refl->GetInt32(message, field_des), value, 10);
					break;
				case FieldDescriptor::TYPE_UINT32:
					_itoa(refl->GetUInt32(message, field_des), value, 10);
					break;
				case FieldDescriptor::TYPE_INT64:
					_i64toa(refl->GetInt64(message, field_des), value, 10);
					break;
				case FieldDescriptor::TYPE_UINT64:
					_ui64toa(refl->GetUInt64(message, field_des), value, 10);
					break;
				default:
					break;
				}
				str_value->vec_value.push_back(string(value));
			}
		}
	}
}


google::protobuf::Message* CDynamicParse::createMessage(const std::string& typeName)
{

	google::protobuf::Message* message = NULL;
	const DescriptorPool * des_pool = m_importer->pool();
	const Descriptor* descriptor = des_pool->FindMessageTypeByName(typeName);
	if(descriptor)
	{
		const google::protobuf::Message* prototype = m_dynamic_message_factory->GetPrototype(descriptor);
		if (prototype)
		{
			message = prototype->New();
		}
	}
	return message;
}

void CDynamicParse::delMsgDesc(MsgVec & vec)
{
	for (auto it = vec.begin(); it != vec.end(); ++it)
		delete *it;

	vec.clear();
}

int CDynamicParse::getMsgDesc(const std::string& type_name, MsgVec & vec)
{
	google::protobuf::Message* msg = createMessage(type_name);
	if (msg == NULL)
		return -1;

	const google::protobuf::Descriptor* des = msg->GetDescriptor();
	int count = des->field_count();
	for (int i = 0; i < count; ++i)
	{
		const google::protobuf::FieldDescriptor* field_des = des->field(i);
		string str_name = field_des->name();

		StructValue* str_value = new StructValue;
		str_value->type1 = field_des->label();
		str_value->type2 = field_des->type();
		str_value->key_name = str_name;
		vec.push_back(str_value);
	}

	return 0;
}
 
int CDynamicParse::getMessageContent(const std::string& type_name, istream* input, Map_Message*& map_content)
{
	google::protobuf::Message* msg = createMessage(type_name);
	if (msg == NULL)
		return -1;
	
	if (NULL != input && !msg->ParseFromIstream(input))
		return -1;

	ReadDesAndInsert(*msg, map_content);

	return 0;
}

	
void CDynamicParse::deleteMessageContent(Map_Message*& map_content)
{
	if (map_content == NULL)
	{
		return;
	}
	deleteMsg(map_content);
}

void CDynamicParse::deleteMsg(Map_Message*& map_content)
{
	for (Map_Message::iterator iter = map_content->begin(); iter != map_content->end(); ++iter)
	{
		vector<Map_Message*>& vec = iter->second->vec_message;
		if (!vec.empty())
		{
			for (vector<Map_Message*>::iterator iter_child = vec.begin(); iter_child != vec.end(); ++iter_child)
			{
				deleteMsg(*iter_child);
			}
		}
		if (iter->second != NULL)
		{
			delete iter->second;
		}
	}
	map_content->clear();
}