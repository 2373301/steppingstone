#ifndef dynamicparseimp_h__
#define dynamicparseimp_h__

#include <google/protobuf/compiler/importer.h>
#include "Singleton.h"
#include "dynamicparse.h"

class DynamicParserImp
	: public IDynamicParser
{
public:
	DynamicParserImp();
	~DynamicParserImp();

	// 可以是相对路径或者绝对路径
	virtual int init(const std::string& proto_path) override;

	virtual int getMessageContent(const std::string& type_name, std::istream* input, MessageMap*& map_content) override;
	virtual void deleteMessageContent(MessageMap*& map_content) override;

	virtual CacheAssistantx * create(uint64 guid, const std::string& name) override;


	// 查询第一层的结构
	virtual int getMsgDesc(const std::string& type_name, MsgVec & vec) override;
	virtual void delMsgDesc(MsgVec & vec) override;

	// 查询db中的table struct
	virtual int querydbDesc(const std::string& a_ip, uint32 a_port, const std::string& a_user, const std::string a_pwd,
		const std::string& a_db_name, TableColumnsMap & a_outcols) override;

	virtual int checkEntity(const TableColumnsMap & a_outcols,
		const std::string& a_db_name, std::vector<std::string>& a_changed) override;

	virtual int updatedbChanged(const std::string& a_ip, uint32 a_port, const std::string& a_user, const std::string a_pwd,
		const std::string& a_db_name, std::vector<std::string>& a_changed) override;

	virtual const google::protobuf::Message* GetPrototype(const std::string& type_name) override;

private:
	google::protobuf::Message* createMessage(const std::string& typeName);
	void ReadDesAndInsert(const google::protobuf::Message& message, MessageMap*& map_content);
	void deleteMsg(MessageMap*& map_content);
private:
	google::protobuf::compiler::Importer * m_importer = NULL;
	google::protobuf::DynamicMessageFactory* m_dynamic_message_factory = NULL;
	std::vector<std::string> m_protofiles;
};


#endif // dynamicparseimp_h__
