#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <mysql++.h>
#include <mysqld_error.h>
#include "dynamicparseimp.h"


class PError : public google::protobuf::compiler::MultiFileErrorCollector
{
	virtual void AddError(const string& filename, int line, int column,
		const string& message)
	{}
};


DynamicParserImp::DynamicParserImp()
{

}

DynamicParserImp::~DynamicParserImp()
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

int DynamicParserImp::init(const string& proto_path)
{
	m_dynamic_message_factory = new google::protobuf::DynamicMessageFactory();
	if (m_dynamic_message_factory == NULL)
	{
		return -1;
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

	return 0;
}


void DynamicParserImp::ReadDesAndInsert(const google::protobuf::Message& message, 	MessageMap*& map_content)
{
	map_content = new MessageMap;

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
					MessageMap* map_msg;
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
				MessageMap* map_msg;
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


google::protobuf::Message* DynamicParserImp::createMessage(const std::string& typeName)
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

const google::protobuf::Message* DynamicParserImp::GetPrototype(const std::string& type_name)
{
	const google::protobuf::Message* message = NULL;
	const google::protobuf::DescriptorPool * des_pool = m_importer->pool();
	const google::protobuf::Descriptor* descriptor = des_pool->FindMessageTypeByName(type_name);
	if (descriptor)
	{
		message =  m_dynamic_message_factory->GetPrototype(descriptor);
	}

	return message;
}

void DynamicParserImp::delMsgDesc(MsgVec & vec)
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

int DynamicParserImp::checkEntity(const TableColumnsMap & a_outcols, const std::string& a_db_name, std::vector<std::string>& a_changed)
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

int DynamicParserImp::updatedbChanged(const std::string& a_ip, uint32 a_port, const std::string& a_user, const std::string a_pwd,
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

int DynamicParserImp::querydbDesc(const std::string& a_ip, uint32 a_port, const std::string& a_user, const std::string a_pwd,
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

int DynamicParserImp::getMsgDesc(const std::string& type_name, MsgVec & vec)
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
 
int DynamicParserImp::getMessageContent(const std::string& type_name, istream* input, MessageMap*& map_content)
{
	google::protobuf::Message* msg = createMessage(type_name);
	if (msg == NULL)
		return -1;
	
	if (NULL != input && !msg->ParseFromIstream(input))
		return -1;

	ReadDesAndInsert(*msg, map_content);

	return 0;
}

	
void DynamicParserImp::deleteMessageContent(MessageMap*& map_content)
{
	if (map_content == NULL)
	{
		return;
	}
	deleteMsg(map_content);
}

void DynamicParserImp::deleteMsg(MessageMap*& map_content)
{
	for (MessageMap::iterator iter = map_content->begin(); iter != map_content->end(); ++iter)
	{
		vector<MessageMap*>& vec = iter->second->vec_message;
		if (!vec.empty())
		{
			for (vector<MessageMap*>::iterator iter_child = vec.begin(); iter_child != vec.end(); ++iter_child)
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

CacheAssistantx::CacheAssistantx()
{

}

CacheAssistantx::~CacheAssistantx()
{
	delete msg;
}

#define  ADD_REPEATED_NUMBER(T) \
{ \
string temp(colVal.data(), colVal.length());\
stringstream ssm(temp); \
::boost::uint32_t num = 0;\
ssm.read(reinterpret_cast<char*>(&num), sizeof(::boost::uint32_t)); \
for (boost::uint32_t j = 0; j < num; j++) \
{ \
	T val = 0; \
	ssm.read(reinterpret_cast<char*>(&val), sizeof(T)); \
	refl->Add##T(msg, field_des, val); \
} \
}

#define  SERIALIZE_REPEATED_NUMBER(T) \
::boost::uint32_t size = refl->FieldSize(*msg, field_des); \
stringstream ssm; \
ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t)); \
for (::boost::uint32_t i = 0; i < size; i++) { \
	T v = refl->GetRepeated##T(*msg, field_des, i); \
	ssm.write(reinterpret_cast<char*>(&v), sizeof(T)); \
} \
query << ::mysqlpp::quote << ssm.str();

typedef uint32 UInt32;
typedef int32 Int32;
typedef uint64 UInt64;
typedef int64 Int64;

bool CacheAssistantx::load(void* q)
{	
	auto query = *(mysqlpp::Query*)q;
	MsgVec vec;
	if ( -1 == parser->getMsgDesc(msg->GetTypeName(), vec))
		return false;

	using namespace ::std;
	try 
	{
		query.clear();
		query << "SELECT ";
		int col = 0;
		for (auto cell : vec)
		{	
			col++;
			if (1 != col)
				query << ",";
			query << cell->key_name <<" ";
		}

		query << " FROM " << msg->GetTypeName();
		query << " WHERE guid=" << myguid;
		const ::mysqlpp::StoreQueryResult& res = query.store();
		if (res.size() == 0) {
			er_code_ = CAE_NOT_FOUND;
			er_str_ = "Cannot find " + msg->GetTypeName() + "<<" + ::boost::lexical_cast<::std::string>(myguid) + ">>";
			return false;
		}

		using namespace google::protobuf;
		const Descriptor* des = msg->GetDescriptor();
		const Reflection* refl = msg->GetReflection();
		int count = des->field_count();
		for (int i = 0; i < count; ++i)
		{
			const FieldDescriptor* field_des = des->field(i);
			string str_name = field_des->name();

			auto colVal = res.at(0).at(i);
			//repeated ----------------------------------
			if (field_des->is_repeated()
				|| field_des->type() == FieldDescriptor::TYPE_MESSAGE)
			{
				if (field_des->type() == FieldDescriptor::TYPE_STRING
					|| field_des->type() == FieldDescriptor::TYPE_BYTES)
				{	
					string temp(colVal.data(), colVal.length());
					stringstream ssm(temp);
					::boost::uint32_t num = 0;
					ssm.read(reinterpret_cast<char*>(&num), sizeof(::boost::uint32_t));
					for (boost::uint32_t j = 0; j < num; j++)
					{
						::boost::uint32_t size = 0;
						ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
						std::string buf;
						buf.resize(size, 0);
						ssm.read(&buf[0], size);
						std::string temp = colVal;
						refl->AddString(msg, field_des, temp);
					}
				}
				else if (field_des->type() == FieldDescriptor::TYPE_MESSAGE)
				{
					string temp(colVal.data(), colVal.length());
					stringstream ssm(temp);
					::boost::uint32_t num = 0;
					ssm.read(reinterpret_cast<char*>(&num), sizeof(::boost::uint32_t));
					for (boost::uint32_t j = 0; j < num; j++)
					{
						::boost::uint32_t size = 0;
						ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
						std::string buf;
						buf.resize(size, 0);
						ssm.read(&buf[0], size);
						auto uniqueMsg = refl->AddMessage(msg, field_des);
						uniqueMsg->ParseFromString(buf);
					}

				}
				else
				{
					switch (field_des->type())
					{
					case FieldDescriptor::TYPE_INT32:
						ADD_REPEATED_NUMBER(Int32);
						break;
					case FieldDescriptor::TYPE_UINT32:
						ADD_REPEATED_NUMBER(UInt32);
						break;
					case FieldDescriptor::TYPE_INT64:
						ADD_REPEATED_NUMBER(Int64);
						break;
					case FieldDescriptor::TYPE_UINT64:
						ADD_REPEATED_NUMBER(UInt64);
						break;
					default:
						break;
					}
				}
			}
			else
			{
				if (field_des->type() == FieldDescriptor::TYPE_STRING
					|| field_des->type() == FieldDescriptor::TYPE_BYTES)
				{	
					std::string temp = colVal;
					refl->SetString(msg, field_des, temp);
				}
				else if (field_des->type() == FieldDescriptor::TYPE_MESSAGE)
				{	
					string temp(colVal.data(), colVal.length());
					stringstream ssm(temp);
					::boost::uint32_t num = 0;
					ssm.read(reinterpret_cast<char*>(&num), sizeof(::boost::uint32_t));
					for (boost::uint32_t j = 0; j < num; j++)
					{
						::boost::uint32_t size = 0;
						ssm.read(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
						std::string buf;
						buf.resize(size, 0);
						ssm.read(&buf[0], size);
						auto uniqueMsg = refl->MutableMessage(msg, field_des);
						uniqueMsg->ParseFromString(buf);
					}
					
				}
				else
				{
					switch (field_des->type())
					{
					case FieldDescriptor::TYPE_INT32:
						refl->SetInt32(msg, field_des, colVal);
						break;
					case FieldDescriptor::TYPE_UINT32:
						refl->SetUInt32(msg, field_des, colVal);
						break;
					case FieldDescriptor::TYPE_INT64:
						refl->SetInt64(msg, field_des, colVal);
						break;
					case FieldDescriptor::TYPE_UINT64:
						refl->SetUInt64(msg, field_des, colVal);
						break;
					default:
						break;
					}
				}
			}
		}

		return true;
	}
	catch (const ::mysqlpp::BadQuery& er) {
		er_code_ = CAE_INTERNAL;
		er_str_ = er.what();
		return false;
	}
	catch (const ::mysqlpp::Exception& er) {
		er_code_ = CAE_INTERNAL;
		er_str_ = er.what();
		return false;
	}

	return false;
}

bool CacheAssistantx::update(void* query)
{
	return false;
}

bool CacheAssistantx::insert(void* q)
{	
	auto query = *(mysqlpp::Query*)q;

	MsgVec vec;
	if (-1 == parser->getMsgDesc(msg->GetTypeName(), vec))
		return false;

	using namespace ::std;
	try {
		query.clear();
		query << "INSERT INTO ";
		query << msg->GetTypeName();
		query << " SET ";

		using namespace google::protobuf;
		const Descriptor* des = msg->GetDescriptor();
		const Reflection* refl = msg->GetReflection();
		int count = des->field_count();
		for (int i = 0; i < count; ++i)
		{
			const FieldDescriptor* field_des = des->field(i);
			query << field_des->name() << "=";

			//repeated ----------------------------------
			if (field_des->is_repeated()
				|| field_des->type() == FieldDescriptor::TYPE_MESSAGE)
			{
				if (field_des->type() == FieldDescriptor::TYPE_STRING
					|| field_des->type() == FieldDescriptor::TYPE_BYTES)
				{	
					stringstream ssm;
					::boost::uint32_t size = refl->FieldSize(*msg, field_des);
					ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
					for (::boost::uint32_t i = 0; i < size; i++) {
						std::string v = refl->GetRepeatedString(*msg, field_des, i);
						::boost::uint32_t len = v.size() + 1;
						ssm.write(reinterpret_cast<char*>(&len), sizeof(::boost::uint32_t));
						ssm.write(v.data(), len);
					}
					query << ::mysqlpp::quote << ssm.str();
				}
				else if (field_des->type() == FieldDescriptor::TYPE_MESSAGE)
				{
					stringstream ssm;
					::boost::int32_t size = refl->FieldSize(*msg, field_des);
					ssm.write(reinterpret_cast<char*>(&size), sizeof(::boost::uint32_t));
					for (boost::int32_t j = 0; j < size; j++)
					{	
						const auto& cell = refl->GetRepeatedMessage(*msg, field_des, j);
						std::string v = cell.SerializeAsString();
						::boost::uint32_t len = v.size() + 1;
						ssm.write(reinterpret_cast<char*>(&len), sizeof(::boost::uint32_t));
						ssm.write(v.data(), len);
					}

					query << ::mysqlpp::quote << ssm.str();
				}
				else
				{
					switch (field_des->type())
					{
					case FieldDescriptor::TYPE_INT32:
					{
						SERIALIZE_REPEATED_NUMBER(Int32);
						break;
					}
					case FieldDescriptor::TYPE_UINT32:
					{
						SERIALIZE_REPEATED_NUMBER(UInt32);
						break;
					}
					case FieldDescriptor::TYPE_INT64:
					{
						SERIALIZE_REPEATED_NUMBER(Int64);
						break;
					}
					case FieldDescriptor::TYPE_UINT64:
					{
						SERIALIZE_REPEATED_NUMBER(UInt64);
						break;
					}
					default:
						break;
					}
				}
			}
			else
			{
				if (field_des->type() == FieldDescriptor::TYPE_STRING
					|| field_des->type() == FieldDescriptor::TYPE_BYTES)
				{
					std::string v = refl->GetString(*msg, field_des);
					query << ::mysqlpp::quote << v;
				}
				else if (field_des->type() == FieldDescriptor::TYPE_MESSAGE)
				{	
					std::stringstream ssm;
					const auto& cell = refl->GetMessage(*msg, field_des);
					std::string v = cell.SerializeAsString();
					::boost::uint32_t len = v.size() + 1;
					ssm.write(reinterpret_cast<char*>(&len), sizeof(::boost::uint32_t));
					ssm.write(v.data(), len);
					query << ::mysqlpp::quote << ssm.str();
				}
				else
				{
					switch (field_des->type())
					{
					case FieldDescriptor::TYPE_INT32:
						query << ::boost::lexical_cast<::std::string>(refl->GetInt32(*msg, field_des));
						break;
					case FieldDescriptor::TYPE_UINT32:
						query << ::boost::lexical_cast<::std::string>(refl->GetUInt32(*msg, field_des));
						break;
					case FieldDescriptor::TYPE_INT64:
						query << ::boost::lexical_cast<::std::string>(refl->GetInt64(*msg, field_des));
						break;
					case FieldDescriptor::TYPE_UINT64:
						query << ::boost::lexical_cast<::std::string>(refl->GetUInt64(*msg, field_des));
						break;
					default:
						break;
					}
				}
			}
		}

	}
	catch (const ::mysqlpp::BadQuery& er) {
		if (ER_DUP_ENTRY == er.errnum())
			er_code_ = CAE_DUP_KEY;
		else
			er_code_ = CAE_INTERNAL;
		er_str_ = er.what();
		return false;
	}
	catch (const ::mysqlpp::Exception& er) {
		er_code_ = CAE_INTERNAL;
		er_str_ = er.what();
		return false;
	}

	return true;
}

bool CacheAssistantx::remove(void* query)
{
	return false;
}

CacheAssistantx* DynamicParserImp::create(uint64 guid, const std::string& name)
{
	google::protobuf::Message* msg = createMessage(name);
	if (msg == NULL)
		return NULL;

	CacheAssistantx *x = new CacheAssistantx;
	x->msg = msg;
	x->parser = this;
	x->myguid = guid;
	return x;
}

IDynamicParser* createDynamicParser()
{
	return new DynamicParserImp();
}

