

#include "ManagePlayerMoney.h"

bool ManagePlayerMoney::isAvailableReduceRequest(typed::protocol::plmsg_line_reduce_gold * reduce_gold)
{
	typed::protocol::plmsg_line_reduce_gold * rg = getReduceGoldInfo(reduce_gold->notify_opcode());
	return NULL == rg ? true : false;
}

bool ManagePlayerMoney::isAvailableUpdateRequest(typed::protocol::plmsg_line_update_reward * update_gold)
{
	typed::protocol::plmsg_line_update_reward * ug = getUpdateGoldInfo(update_gold->notify_opcode());
	return NULL == ug ? true : false;
}

void ManagePlayerMoney::addReduceRequest(typed::protocol::plmsg_line_reduce_gold * reduce_gold)
{
	typed::protocol::plmsg_line_reduce_gold * reduceg = new typed::protocol::plmsg_line_reduce_gold();
	reduceg->CopyFrom(*reduce_gold);

	m_reduce_gold_map.insert(std::make_pair(reduce_gold->notify_opcode(), reduceg));
}

void ManagePlayerMoney::addUpdateRequest(typed::protocol::plmsg_line_update_reward * update_gold)
{
	typed::protocol::plmsg_line_update_reward * updateg = new typed::protocol::plmsg_line_update_reward();
	updateg->CopyFrom(*update_gold);

	m_update_gold_map.insert(std::make_pair(update_gold->notify_opcode(), updateg));
}

typed::protocol::plmsg_line_reduce_gold * ManagePlayerMoney::getReduceGoldInfo(int notify_opcode)
{
	ReduceGoldMap_t::iterator it = m_reduce_gold_map.find(notify_opcode);
	if (it != m_reduce_gold_map.end())
	{
		return it->second;
	}

	return NULL;
}

typed::protocol::plmsg_line_update_reward * ManagePlayerMoney::getUpdateGoldInfo(int notify_opcode)
{
	UpdateGoldMap_t::iterator it = m_update_gold_map.find(notify_opcode);
	if (it != m_update_gold_map.end())
	{
		return it->second;
	}

	return NULL;
}

void ManagePlayerMoney::removeReduceRequest(int notify_code)
{
	ReduceGoldMap_t::iterator it = m_reduce_gold_map.find(notify_code);
	if (it != m_reduce_gold_map.end())
	{
		delete it->second;
		m_reduce_gold_map.erase(it);
	}
}

void ManagePlayerMoney::removeUpdateRequest(int notify_code)
{
	UpdateGoldMap_t::iterator it = m_update_gold_map.find(notify_code);
	if (it != m_update_gold_map.end())
	{
		delete it->second;
		m_update_gold_map.erase(it);
	}
}

bool ManagePlayerMoney::isEmpty()
{
	if ((m_reduce_gold_map.size() == 0)
		&& (m_update_gold_map.size() == 0))
	{
		return true;
	}

	return false;
}