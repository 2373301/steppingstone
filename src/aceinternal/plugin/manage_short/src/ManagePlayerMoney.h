

#ifndef MANAGE_PLAYER_MONEY_HPP
#define MANAGE_PLAYER_MONEY_HPP

#include "typedef.h"
#include "coredef.h"
#include "protocol_ins.h"
#include "entity_ins.h"

class ManagePlayerMoney
{
public:
	bool isAvailableReduceRequest(typed::protocol::plmsg_line_reduce_gold * reduce_gold);

	bool isAvailableUpdateRequest(typed::protocol::plmsg_line_update_reward * update_gold);

	void addReduceRequest(typed::protocol::plmsg_line_reduce_gold * reduce_gold);

	void addUpdateRequest(typed::protocol::plmsg_line_update_reward * update_gold);

	typed::protocol::plmsg_line_reduce_gold * getReduceGoldInfo(int notify_opcode);

	typed::protocol::plmsg_line_update_reward * getUpdateGoldInfo(int notify_opcode);

	void removeReduceRequest(int notify_code);

	void removeUpdateRequest(int notify_code);

	bool isEmpty();
protected:

private:
	typedef define_unordered_map<int, typed::protocol::plmsg_line_reduce_gold *> ReduceGoldMap_t;

	typedef define_unordered_map<int, typed::protocol::plmsg_line_update_reward *> UpdateGoldMap_t;

	ReduceGoldMap_t m_reduce_gold_map;

	UpdateGoldMap_t m_update_gold_map;
};

#endif