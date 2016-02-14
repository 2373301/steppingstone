
#include <fstream>
#include <boost/lexical_cast.hpp>
#include "LocalCache.h"
#include "cachesys_opcode.hpp"
#include "Logger.h"
#include "coredef.h"
#include "protocol/msg_cache.pb.h"
#include "protocol/msg_pool.pb.h"

string empty_stream = "";

LocalCache::LocalCache()
{

}

int LocalCache::wt_stream()
{
	return 0;
}

int LocalCache::finit()
{
	return 0;
}

void LocalCache::output(Packet * packet)
{
	handlePacket(packet);
}

int LocalCache::initing()
{
	boost::filesystem::path p(m_local_path);
	if (!boost::filesystem::exists(p))
	{
		// create dir
		if (!boost::filesystem::create_directories(p))
		{
			DEF_LOG_ERROR("failed to crate dir <%s>\n", m_local_path.c_str());
			return -1;
		}
	}

	m_player_path = p / "player";
	if ((!boost::filesystem::exists(m_player_path)) 
		&& (!boost::filesystem::create_directories(m_player_path)))
	{
		DEF_LOG_ERROR("failed to crate dir <%s>\n", m_player_path.string().c_str());
		return -1;
	}

	return 0;
}

int LocalCache::net_connected()
{
	return 0;
}

int LocalCache::net_closed()
{
	return 0;
}

void LocalCache::handlePacket(Packet * packet)
{
	if (DCMSG_NEW_ENEITY == packet->opcode())
	{
		handleAddEntity(packet);
	}
	else if (DCMSG_LOAD_ENTITY == packet->opcode())
	{
		handleLoadEntity(packet);
	}
	else if (DCMSG_QUERY_ENTITY == packet->opcode())
	{
		handleQueryEntity(packet);
	}
	else if (DCMSG_UPDATE_ENTITY == packet->opcode())
	{
		handleUpdateEntity(packet);
	}
	else if (DCMSG_FLUSH_ENTITY == packet->opcode())
	{
		handleFlushEntity(packet);
	}
	else if (DCMSG_REMOVE_ENTITY == packet->opcode())
	{
		handleDeleteEntity(packet);
	}
	else
	{
		// error
	}

	delete packet;
}

int LocalCache::handleAddEntity(Packet * packet)
{
	fs::path entity_path;
	if (getEntityPath(packet->guid(), entity_path))
	{
		addEntity(m_player_path, packet->guid(), string(packet->ch_body(), packet->body_size()));
	}
	return 0;
}

int LocalCache::handleLoadEntity(Packet * packet)
{
	fs::path entity_path;
	if (getEntityPath(packet->guid(), entity_path))
	{
		loadEntity(m_player_path, packet->guid());
	}
	return 0;
}

int LocalCache::handleQueryEntity(Packet * packet)
{
	return handleLoadEntity(packet);
}

int LocalCache::handleUpdateEntity(Packet * packet)
{
	fs::path entity_path;
	if (getEntityPath(packet->guid(), entity_path))
	{
		updateEntity(m_player_path, packet->guid(), string(packet->ch_body(), packet->body_size()));
	}
	return 0;
}

int LocalCache::handleFlushEntity(Packet * packet)
{
	fs::path entity_path;
	if (getEntityPath(packet->guid(), entity_path))
	{
		flushEntity(m_player_path, packet->guid(), string(packet->ch_body(), packet->body_size()));
	}
	return 0;
}

int LocalCache::handleDeleteEntity(Packet * packet)
{
	fs::path entity_path;
	if (getEntityPath(packet->guid(), entity_path))
	{
		deleteEntity(m_player_path, packet->guid());
	}
	return 0;
}

bool LocalCache::loadEntity(const fs::path & file_dir, uint64 guid)
{
	fs::path file_path = file_dir / boost::lexical_cast<string>(guid);
	if (boost::filesystem::exists(file_path))
	{
		fstream f;
		f.open(file_path.string().c_str(), ios_base::in | ios_base::binary);
		f.seekg(0, ios_base::end);
		int file_size = f.tellg();
		string stream;
		stream.resize(file_size, 0);
		f.seekg(0, ios_base::beg);
		f.read((char *)stream.data(), file_size);
		f.close();
		returnOpResult(DCMSG_LOAD_ENTITY, true, guid, stream);
		return true;
	}
	else
	{
		returnOpResult(DCMSG_LOAD_ENTITY, false, guid, empty_stream);
		return false;
	}
}

bool LocalCache::addEntity(const fs::path & file_dir, uint64 guid, const string & stream)
{
	fs::path file_path = file_dir / boost::lexical_cast<string>(guid);
	if (!boost::filesystem::exists(file_path))
	{
		fstream f;
		f.open(file_path.string().c_str(), ios_base::trunc | ios_base::out | ios_base::binary);
		f.write(stream.data(), stream.size());
		f.close();

		returnOpResult(DCMSG_NEW_ENEITY, true, guid, empty_stream);

		return true;
	}
	else
	{
		// error
		returnOpResult(DCMSG_NEW_ENEITY, false, guid, empty_stream);

		return false;
	}
}

bool LocalCache::updateEntity(const fs::path & file_dir, uint64 guid, const string & stream)
{
	fs::path file_path = file_dir / boost::lexical_cast<string>(guid);
	if (boost::filesystem::exists(file_path))
	{
		fstream f;
		f.open(file_path.string().c_str(), ios_base::trunc | ios_base::out | ios_base::binary);
		f.write(stream.data(), stream.size());
		f.close();

		returnOpResult(DCMSG_UPDATE_ENTITY, true, guid, empty_stream);

		return true;
	}
	else
	{
		returnOpResult(DCMSG_UPDATE_ENTITY, false, guid, empty_stream);

		return false;
	}
}

bool LocalCache::flushEntity(const fs::path & file_dir, uint64 guid, const string & stream)
{
	fs::path file_path = file_dir / boost::lexical_cast<string>(guid);

	fstream f;
	f.open(file_path.string().c_str(), ios_base::trunc | ios_base::out | ios_base::binary);
	f.write(stream.data(), stream.size());
	f.close();

	returnOpResult(DCMSG_FLUSH_ENTITY, true, guid, empty_stream);

	return true;
}

bool LocalCache::deleteEntity(const fs::path & file_dir, uint64 guid)
{
	fs::path file_path = file_dir / boost::lexical_cast<string>(guid);
	if (boost::filesystem::exists(file_path))
	{
		if (boost::filesystem::remove(file_path))
		{
			returnOpResult(DCMSG_REMOVE_ENTITY, true, guid, empty_stream);
			return true;
		}
		else
		{
			returnOpResult(DCMSG_REMOVE_ENTITY, false, guid, empty_stream);
			return false;
		}
	}
	else
	{
		// error
		returnOpResult(DCMSG_REMOVE_ENTITY, false, guid, empty_stream);
		return false;
	}
}

bool LocalCache::getEntityPath(uint64 guid, fs::path & file_dir)
{
	bool result = false;
	switch(EXTRACT_ENTITY_TYPE(guid))
	{
	case ET_PLAYER:
		file_dir = m_player_path;
		result = true;
		break;
	default:
		// error
		break;
	}

	return result;
}

void LocalCache::returnOpResult(int op_type, bool result, uint64 guid, const string & stream)
{
	//protocol::OpResult op_result;
	//op_result.set_success(result);
	//if (stream.size() > 0)
	//{
	//	op_result.set_stream_data(stream);
	//}

	typed::cached::protocol::Result op_result;
	if (result)
	{
		op_result.set_status(typed::cached::protocol::Result_status_t_STATE_NORMAL);
		op_result.set_message(stream);
	}
	else
	{
		op_result.set_status(typed::cached::protocol::Result_status_t_STATE_INTERNAL_ERROR);
	}

	// todo
	//Packet * ps = new Packet(op_type, guid, op_result.SerializeAsString());
	//m_handle_input->input(ps);
}