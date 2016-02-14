
#ifndef LOCAL_CACHE_HPP
#define LOCAL_CACHE_HPP
#include <boost/filesystem.hpp>

#include "Cache.h"

namespace fs = boost::filesystem;

class LocalCache : public Cache
{
public:
	LocalCache();

public:
	virtual int wt_stream();

public:
	virtual int finit();

	virtual void output(Packet * packet);

protected:
	virtual int initing();

	virtual int net_connected();

	virtual int net_closed();

	void handlePacket(Packet * packet);

protected:

	int handleAddEntity(Packet * packet);

	int handleLoadEntity(Packet * packet);

	int handleQueryEntity(Packet * packet);

	int handleUpdateEntity(Packet * packet);

	int handleFlushEntity(Packet * packet);

	int handleDeleteEntity(Packet * packet);

protected:
	bool loadEntity(const fs::path & file_dir, uint64 guid);

	bool addEntity(const fs::path & file_dir, uint64 guid, const string & stream);

	bool updateEntity(const fs::path & file_dir, uint64 guid, const string & stream);

	bool flushEntity(const fs::path & file_dir, uint64 guid, const string & stream);

	bool deleteEntity(const fs::path & file_dir, uint64 guid);

	bool getEntityPath(uint64 guid, fs::path & file_dir);

	void returnOpResult(int op_type, bool result, uint64 guid, const string & stream);
private:
	fs::path m_player_path;
};

#endif