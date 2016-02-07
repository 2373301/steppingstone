
#ifndef MANAGE_CONFIG_HPP
#define MANAGE_CONFIG_HPP

#include "SimpleXML.h"

#include "SingletonInit.h"
#include "Singleton.h"

struct GuidCfg
{
	GuidCfg()
		: seed_value(0)
		, index(0)
	{}

	uint32 seed_value;
	uint64 index;
	string addr;
};

class ManageConfig : public SingletonInit
{
public:
	ManageConfig();

	~ManageConfig();
public:
	static ManageConfig * instance()
	{
		return Singleton<ManageConfig>::instance();
	}
public:
	virtual int init();

	virtual int stop();

	virtual int finit();
public:
	const GuidCfg & getGuidCfg();

	bool writeGuidCfg(uint32 seed_value, uint64 index);
protected:
	bool loadConfig(const string & cfg_path);

private:
	Document * m_config_doc;

	Attribute * m_seed_attr;

	Attribute * m_index_attr;

	GuidCfg m_guid_cfg;
};
#endif