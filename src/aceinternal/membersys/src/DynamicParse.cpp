//dynamic_parse.cpp
#include "DynamicParse.h"

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


DynamicParse::DynamicParse()
{

}

DynamicParse::~DynamicParse()
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

int DynamicParse::init(const string& proto_path)
{
	m_dynamic_message_factory = new DynamicMessageFactory();
	if (m_dynamic_message_factory == NULL)
	{
		return -1;
	}
	PError errorCollector;
	google::protobuf::compiler::DiskSourceTree sourceTree;
	/*google::protobuf::compiler::Importer*/ 
	m_importer = new google::protobuf::compiler::Importer(&sourceTree, &errorCollector); 
	sourceTree.MapPath("", /*"D:\\working\\apss\\trunk\\common\\material\\protocol\\"*/proto_path); 


	boost::filesystem::path fsPath(/*"D:\\working\\apss\\trunk\\common\\material\\protocol\\"*/proto_path/* ,boost::filesystem::native*/);
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

	return 0;
}


void DynamicParse::ReadDesAndInsert(const google::protobuf::Message& message, 	Map_Message*& map_content)
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
//			HTREEITEM hSubItem = m_tree.InsertItem(str_name.c_str(),hItem);
			int field_size = refl->FieldSize(message, field_des);
			for (int j=0; j<field_size; ++j)
			{	

	//			CString str_show;

				if (field_des->type() == FieldDescriptor::TYPE_STRING 
					|| field_des->type() == FieldDescriptor::TYPE_BYTES)
				{
					//str_show.Format("[%d]%s", j, (refl->GetRepeatedString(message, field_des, j)).c_str());
					//hSubItem = m_tree.InsertItem((LPCSTR)str_show, hSubItem);

					str_value->vec_value.push_back(refl->GetRepeatedString(message, field_des, j));
				}
				else if (field_des->type() == FieldDescriptor::TYPE_MESSAGE)
				{
					//str_show.Format("[%d]", j);
					//HTREEITEM hItem_temp = m_tree.InsertItem((LPCSTR)str_show, hSubItem);
					const google::protobuf::Message& msg_sub = refl->GetRepeatedMessage(message, field_des, j);
					//ReadDesAndInsert(msg_sub, hItem_temp);

					Map_Message* map_msg;
					ReadDesAndInsert(msg_sub, map_msg);
					str_value->vec_message.push_back(map_msg);
				}
				else
				{
					char value[128] = {0};
			//		uint64 temp_value = 0;
					switch (field_des->type())
					{
					case FieldDescriptor::TYPE_INT32:
						//itoa(refl->GetRepeatedInt32(message, field_des, j), value, 10);
						sprintf(value, "%d", refl->GetRepeatedInt32(message, field_des, j));
						break;
					case FieldDescriptor::TYPE_UINT32:
						//itoa(refl->GetRepeatedUInt32(message, field_des, j), value, 10);
						sprintf(value, "%u", refl->GetRepeatedUInt32(message, field_des, j));
						break;
					case FieldDescriptor::TYPE_INT64:
						//_i64toa(refl->GetRepeatedInt64(message, field_des, j), value, 10);
						sprintf(value, "%lld", refl->GetRepeatedInt64(message, field_des, j));
						break;
					case FieldDescriptor::TYPE_UINT64:
						//_ui64toa(refl->GetRepeatedUInt64(message, field_des, j), value, 10);
						sprintf(value, "%llu", refl->GetRepeatedUInt64(message, field_des, j));
						break;
					default:
						break;
					}
					//str_show.Format("[%d]%I64u", j, temp_value);
					//m_tree.InsertItem((LPCSTR)str_show, hSubItem);
			//		str_show.Format("%I64u", temp_value);
			//		string val_temp = str_show;
					str_value->vec_value.push_back(string(value));
				}
			}
		}
		else
		{
//			CString str_show;
			if (field_des->type() == FieldDescriptor::TYPE_STRING 
				|| field_des->type() == FieldDescriptor::TYPE_BYTES)
			{
				//str_show.Format("[%s]%s", str_name.c_str(), (refl->GetString(message, field_des)).c_str());
				//m_tree.InsertItem((LPCSTR)str_show, hItem);
				str_value->vec_value.push_back(refl->GetString(message, field_des));
			}
			else if (field_des->type() == FieldDescriptor::TYPE_MESSAGE)
			{
				//str_show.Format("%s", str_name.c_str());
				//HTREEITEM hSubItem = m_tree.InsertItem((LPCSTR)str_show, hItem);
				const google::protobuf::Message& msg_sub = refl->GetMessage(message, field_des);
				//ReadDesAndInsert(msg_sub, hSubItem);

				Map_Message* map_msg;
				ReadDesAndInsert(msg_sub, map_msg);
				str_value->vec_message.push_back(map_msg);
			}
			else
			{
				char value[128] = {0};
			//	string val_temp;
			//	ULONGLONG temp_value = 0;
				switch (field_des->type())
				{
				case FieldDescriptor::TYPE_INT32:
					//_itoa(refl->GetInt32(message, field_des), value, 10);
					sprintf(value, "%d", refl->GetInt32(message, field_des));
					break;
				case FieldDescriptor::TYPE_UINT32:
					//_itoa(refl->GetUInt32(message, field_des), value, 10);
					sprintf(value, "%u", refl->GetUInt32(message, field_des));
					break;
				case FieldDescriptor::TYPE_INT64:
					//_i64toa(refl->GetInt64(message, field_des), value, 10);
					sprintf(value, "%llu", refl->GetInt64(message, field_des));
					break;
				case FieldDescriptor::TYPE_UINT64:
					//_ui64toa(refl->GetUInt64(message, field_des), value, 10);
					sprintf(value, "%llu", refl->GetUInt64(message, field_des));
					break;
				default:
					break;
				}
				//str_show.Format("[%s]%I64u", str_name.c_str(), temp_value);
				//m_tree.InsertItem((LPCSTR)str_show, hItem);

			//	str_show.Format("%I64u", temp_value);
			//	string val_temp = str_show;
				str_value->vec_value.push_back(string(value));
			}
		}
	}
}


google::protobuf::Message* DynamicParse::createMessage(const std::string& typeName)
{

	google::protobuf::Message* message = NULL;
	const DescriptorPool * des_pool = m_importer->pool();
	const Descriptor* descriptor = des_pool->FindMessageTypeByName(typeName);
	//	DynamicMessageFactory * dynamicMessageFactory=new DynamicMessageFactory();
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
 
int DynamicParse::getMessageContent(const std::string& type_name, istream* input, Map_Message*& map_content)
{
	google::protobuf::Message* msg = createMessage(type_name);
	if (msg == NULL)
	{
		return -1;
	}
	msg->ParseFromIstream(input);

	ReadDesAndInsert(*msg, map_content);

	return 0;
}

int DynamicParse::getMessageContent(google::protobuf::Message* msg, Map_Message*& map_content)
{
	ReadDesAndInsert(*msg, map_content);
	return 0;
}
	
void DynamicParse::deleteMessageContent(Map_Message*& map_content)
{
	if (map_content == NULL)
	{
		return;
	}
	deleteMsg(map_content);
}

void DynamicParse::getJsonFormat(google::protobuf::Message * msg, string & json_format)
{
	Map_Message * map_content = NULL;
	getMessageContent(msg, map_content);
	json_format += "{";
	getMessageContent(map_content, json_format);
	json_format += "}";
	deleteMsg(map_content);
}

void DynamicParse::getMessageContent(Map_Message * map_content, string & json_format)
{
	int index = 0;
	for (Map_Message::iterator it = map_content->begin(); it != map_content->end(); ++it, ++index)
	{
		if (index > 0)
		{
			json_format += ", ";
		}
		const string & name = it->first;
		StructValue * info = it->second;
		json_format += "\"" + name + "\":";
		if (info->type1 == google::protobuf::FieldDescriptor::LABEL_REPEATED)
		{
			json_format += "[";
			for (int i = 0; i < info->vec_message.size(); ++i)
			{
                if (0 == i)
                {
				    json_format += "{";
                }
                else
                {
                    json_format += ",{";
                }
				getMessageContent(info->vec_message[i], json_format);
				json_format += "}";
			}
			json_format += "]";
		}
		else
		{
			json_format += "\"";
			json_format += info->vec_value[0];
			json_format += "\"";
		}
	}
}

void DynamicParse::deleteMsg(Map_Message*& map_content)
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