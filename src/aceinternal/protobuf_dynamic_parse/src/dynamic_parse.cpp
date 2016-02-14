//dynamic_parse.cpp
#include "dynamic_parse.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <mysql++.h>

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
	m_dynamic_message_factory = new google::protobuf::DynamicMessageFactory();
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
			m_protofiles.push_back(str_name);
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
				if (field_des->type() == google::protobuf::FieldDescriptor::TYPE_STRING
					|| field_des->type() == google::protobuf::FieldDescriptor::TYPE_BYTES)
				{
					str_value->vec_value.push_back(refl->GetRepeatedString(message, field_des, j));
				}
				else if (field_des->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE)
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
					case google::protobuf::FieldDescriptor::TYPE_INT32:
						_itoa(refl->GetRepeatedInt32(message, field_des, j), value, 10);
						break;
					case google::protobuf::FieldDescriptor::TYPE_UINT32:
						_itoa(refl->GetRepeatedUInt32(message, field_des, j), value, 10);
						break;
					case google::protobuf::FieldDescriptor::TYPE_INT64:
						_i64toa(refl->GetRepeatedInt64(message, field_des, j), value, 10);
						break;
					case google::protobuf::FieldDescriptor::TYPE_UINT64:
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
			if (field_des->type() == google::protobuf::FieldDescriptor::TYPE_STRING
				|| field_des->type() == google::protobuf::FieldDescriptor::TYPE_BYTES)
			{
				str_value->vec_value.push_back(refl->GetString(message, field_des));
			}
			else if (field_des->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE)
			{	
				#undef GetMessage 
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
				case google::protobuf::FieldDescriptor::TYPE_INT32:
					_itoa(refl->GetInt32(message, field_des), value, 10);
					break;
				case google::protobuf::FieldDescriptor::TYPE_UINT32:
					_itoa(refl->GetUInt32(message, field_des), value, 10);
					break;
				case google::protobuf::FieldDescriptor::TYPE_INT64:
					_i64toa(refl->GetInt64(message, field_des), value, 10);
					break;
				case google::protobuf::FieldDescriptor::TYPE_UINT64:
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
	const google::protobuf::DescriptorPool * des_pool = m_importer->pool();
	const google::protobuf::Descriptor* descriptor = des_pool->FindMessageTypeByName(typeName);
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


static std::string getTableFieldType(const google::protobuf::FieldDescriptor*  field_des)
{
	std::string temp_field = "";
	if (field_des->is_repeated())
		temp_field = "blob ";
	else
	{
		switch (field_des->type())
		{
		case google::protobuf::FieldDescriptor::TYPE_INT32:
		case google::protobuf::FieldDescriptor::TYPE_BOOL:
			temp_field = "int(11) ";
			break;
		case google::protobuf::FieldDescriptor::TYPE_UINT32:
			temp_field = "int(11) unsigned ";
			break;
		case google::protobuf::FieldDescriptor::TYPE_INT64:
			temp_field = "bigint(20) ";
			break;
		case google::protobuf::FieldDescriptor::TYPE_UINT64:
			temp_field = "bigint(20) unsigned ";
			break;
		case google::protobuf::FieldDescriptor::TYPE_STRING:
		case google::protobuf::FieldDescriptor::TYPE_BYTES:
			temp_field = "text ";
			break;
		case google::protobuf::FieldDescriptor::TYPE_MESSAGE:
			temp_field = "blob ";
			break;
		default:
			break;
		}
	}
	return temp_field;
}
static std::string getTableFieldIsNull(const google::protobuf::FieldDescriptor*  field_des)
{
	std::string temp_isnull = "NULL ";
	if (field_des->is_required())
		temp_isnull = "NOT NULL ";
	return temp_isnull;
}

static std::string getTableFieldDefault(const google::protobuf::FieldDescriptor*  field_des)
{
	std::string temp_default = "";
	if (!field_des->is_repeated())
	{
		switch (field_des->type())
		{
		case google::protobuf::FieldDescriptor::TYPE_BOOL:
		case google::protobuf::FieldDescriptor::TYPE_INT32:
		case google::protobuf::FieldDescriptor::TYPE_UINT32:
		case google::protobuf::FieldDescriptor::TYPE_INT64:
		case google::protobuf::FieldDescriptor::TYPE_UINT64:
			temp_default = "DEFAULT 0 ";
			break;
		default:
			break;
		}
	}
	return temp_default;
}

int CDynamicParse::checkEntity(const TableColumnsMap & a_outcols, const std::string& a_db_name, std::vector<std::string>& a_changed)
{
	std::stringstream fn;

	for (auto itr = m_protofiles.begin(); itr != m_protofiles.end(); ++itr)
	{
		std::string protofile = *itr;
		const google::protobuf::DescriptorPool* p_pool = m_importer->pool();
		const google::protobuf::FileDescriptor* p_file = p_pool->FindFileByName(protofile);
		int msg_count = p_file->message_type_count();
		for (int i = 0; i < msg_count; ++i)
		{
			const google::protobuf::Descriptor* descriptor = p_file->message_type(i);
			if (NULL == descriptor)
			{
				printf("not found message<%d> in %s ", i, protofile.c_str());
				continue;
			}
			
			auto message = m_dynamic_message_factory->GetPrototype(descriptor)->New();
			std::string msg_name = message->GetTypeName();
			auto pos2 = msg_name.find_last_of(".");
			if (pos2 != std::string::npos)
				msg_name = msg_name.erase(0, pos2 + 1);

			auto des = message->GetDescriptor();
			int field_count = des->field_count();

			auto found = a_outcols.find(msg_name);
			if (found != a_outcols.end())
			{
				//alter table
				for (int j = 0; j < field_count; ++j)
				{
					const google::protobuf::FieldDescriptor* field_des = des->field(j);
					string field_name = field_des->name();
					if (found->second.find(field_name) != found->second.end())
						continue;

					fn << "ALTER TABLE " << a_db_name << "." << msg_name << " ";
					fn << "ADD COLUMN " 
						<< field_name << " ";
					fn << getTableFieldType(field_des);
					fn << getTableFieldDefault(field_des);
					fn << getTableFieldIsNull(field_des);
					if (j > 0)
					{
						const google::protobuf::FieldDescriptor* front_field_des = des->field(j - 1);
						string front_fieldname = front_field_des->name();
						fn << "AFTER " << front_fieldname << ";\n";
					}

					a_changed.push_back(fn.str());
					fn.str("");

					if (field_des->is_repeated())
					{
						fn << "UPDATE " << a_db_name << "." << msg_name << " ";
						fn << "SET " << field_name << " = 0x00000000 ;\n";

						a_changed.push_back(fn.str());
						fn.str("");
					}
					
				}
			}
			else
			{
				//add table
				std::string alter_info = "\nDROP TABLE IF EXISTS " + a_db_name + "." + msg_name + ";\n";
				a_changed.push_back(alter_info);
				alter_info.clear();

				alter_info += "CREATE TABLE " + a_db_name + "." + msg_name + " (\n";
				for (int j = 0; j < field_count; ++j)
				{
					const google::protobuf::FieldDescriptor* field_des = des->field(j);
					string field_name = field_des->name();
					alter_info += "" + field_name + " ";
					alter_info += getTableFieldType(field_des);
					alter_info += getTableFieldIsNull(field_des);
					alter_info += getTableFieldDefault(field_des);
					alter_info += ",\n";
				}
				alter_info += "PRIMARY KEY (guid)\n";
				alter_info += ") ENGINE=InnoDB DEFAULT CHARSET=utf8;\n\n";

				a_changed.push_back(alter_info);
			}

		} // msg loop in proto file

	} // proto files loop

	return 0;
}

int CDynamicParse::updatedbChanged(const std::string& a_ip, uint32 a_port, const std::string& a_user, const std::string a_pwd,
	const std::string& a_db_name, std::vector<std::string>& a_changed)
{	
	using namespace ::std;

	try {

		mysqlpp::Connection conn(true);
		conn.connect(a_db_name.c_str(), a_ip.c_str(), a_user.c_str(), a_pwd.c_str(), a_port);
		for (auto it = a_changed.begin(); it != a_changed.end(); ++it)
		{
			mysqlpp::Query table_query = conn.query();
			table_query << *it;
			table_query.exec();
		}

	}
	catch (const ::mysqlpp::BadQuery& er) {
		printf("DB connection failed:%s\n, db name:%s, ip:%s, port:%u, user:%s, pwd:%s",
			er.what(), a_db_name.c_str(), a_ip.c_str(), a_port, a_user.c_str(), a_pwd.c_str());
		return -1;
	}
	catch (const ::mysqlpp::Exception& er) {
		printf("DB connection failed:%s\n, db name:%s, ip:%s, port:%u, user:%s, pwd:%s",
			er.what(), a_db_name.c_str(), a_ip.c_str(), a_port, a_user.c_str(), a_pwd.c_str());
		return -1;
	}

	return 0;
}

int CDynamicParse::querydbDesc(const std::string& a_ip, uint32 a_port, const std::string& a_user, const std::string a_pwd,
	const std::string& a_db_name, TableColumnsMap & a_outcols)
{	
	mysqlpp::Connection conn(false);
	if ( !conn.connect(a_db_name.c_str(), a_ip.c_str(), a_user.c_str(), a_pwd.c_str(), a_port))
	{
		printf("DB connection failed, db name:%s, ip:%s, port:%u, user:%s, pwd:%s",
			a_db_name.c_str(), a_ip.c_str(), a_port, a_user.c_str(), a_pwd.c_str());
		return -1;
	}

	std::string table_sql = "SHOW TABLES FROM " + a_db_name;
	mysqlpp::Query table_query = conn.query(table_sql);
	const mysqlpp::StoreQueryResult& table_res = table_query.store();
	for (boost::uint32_t i = 0; i < table_res.size(); ++i)
	{
		std::string table_name = table_res.at(i).at(0);
		std::string columns_sql = "SHOW COLUMNS FROM " + table_name;
		mysqlpp::Query columns_query = conn.query(columns_sql);
		const mysqlpp::StoreQueryResult& columns_res = columns_query.store();
		for (boost::uint32_t j = 0; j < columns_res.size(); ++j)
		{
			std::string column_name = columns_res.at(j).at(0);
			TableColumnsMap::iterator found = a_outcols.find(table_name);
			if (found == a_outcols.end())
			{
				std::set<std::string> columns_set;
				columns_set.insert(column_name);
				a_outcols.insert(std::make_pair(table_name, columns_set));
			}
			else
			{
				found->second.insert(column_name);
			}
		}
	}

	return 0;
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