
#ifndef PLAYER_FUNCTION_HPP
#define PLAYER_FUNCTION_HPP

#include "coredef.h"
#include "entity_ins.h"
#include "protocol_ins.h"
#include "gamelogic_def.h"
#include "Resource.h"
#include "Packet.h"

#define CHECK_ITEM_GUID(guid)	\
	if (1 == (guid))	\
		continue;	\
	else if (0 == (guid))	\
		break;

#define CHECK_ITEM_AVAILABLE_POS(guid, if_not_cmd)	\
	if (1 != guid)	\
	{	\
	if_not_cmd;	\
	}

class Scene;

GAMELOGIC_EXOPRT uint64 getCurrentTimeInSec();

GAMELOGIC_EXOPRT uint32 getTimeDifferentDay(uint64 big_time_sec, uint64 small_time_sec);

GAMELOGIC_EXOPRT int getRoleAttr(typed::entity::Role * role, RoleAttrType role_attr);
//
//GAMELOGIC_EXOPRT bool calculateFormBonus(Scene * scene, typed::entity::Role * role, FormResInfo * form_info, bool notify_client);
//
//GAMELOGIC_EXOPRT bool calculateFormBonus(Scene * scene, vector<typed::entity::Role *> & roles, FormResInfo * form_info, bool notify_client);

GAMELOGIC_EXOPRT void fillClientItemContent(typed::entity::Item * item, typed::protocol::smsg_create_item * create_item);

GAMELOGIC_EXOPRT GOOGLE_MESSAGE_TYPE * cloneMessage(GOOGLE_MESSAGE_TYPE * msg);

GAMELOGIC_EXOPRT int get_UTF8_strlen(const char* src, int length);

GAMELOGIC_EXOPRT void fillRoleAttrRank(::google::protobuf::RepeatedField< ::google::protobuf::uint32 > * attributes, typed::entity::Role * role, typed::entity::Player * player, SystemResInfo * system_res_info);

GAMELOGIC_EXOPRT int getAttrRank(typed::entity::Role * role, SystemResInfo * system_res_info, int attr_index, int attr_rank_index, int player_level);

// only support base type
// return index(base 0) if find, return -1 if do not find
template<class BASE_TYPE>
int findInRepeatedField(BASE_TYPE find_value, ::google::protobuf::RepeatedField<BASE_TYPE> & repeated_value);

// return true if remove the item of first finding
template<class BASE_TYPE>
bool removeFirstInRepeatedField(BASE_TYPE find_value, ::google::protobuf::RepeatedField<BASE_TYPE> & repeated_value);

// return 0 if do not find, return n if remove the item of n
template<class BASE_TYPE>
int removeAllInRepeatedField(BASE_TYPE find_value, ::google::protobuf::RepeatedField<BASE_TYPE> & repeated_value);





template<class BASE_TYPE>
int findInRepeatedField(BASE_TYPE find_value, ::google::protobuf::RepeatedField<BASE_TYPE> & repeated_value)
{
	int result = -1;
	for (int i = 0; i < repeated_value.size(); ++i)
	{
		if (repeated_value.Get(i) == find_value)
		{
			result = i;
			break;
		}
	}

	return result;
}

template<class BASE_TYPE>
bool removeFirstInRepeatedField(BASE_TYPE find_value, ::google::protobuf::RepeatedField<BASE_TYPE> & repeated_value)
{
	bool is_find = false;
	int index = 0;
	for (; index < repeated_value.size(); ++index)
	{
		if (repeated_value.Get(index) == find_value)
		{
			is_find = true;
			break;
		}
	}

	if (is_find)
	{
		for (int i = index; i < repeated_value.size() - 1; ++i)
		{
			repeated_value.Set(i, repeated_value.Get(i + 1));
		}
		repeated_value.RemoveLast();
		return true;
	}
	return false;
}

template<class BASE_TYPE>
int removeAllInRepeatedField(BASE_TYPE find_value, ::google::protobuf::RepeatedField<BASE_TYPE> & repeated_value)
{
	int remove_time = 0;
	::google::protobuf::RepeatedField<BASE_TYPE> copy_value(repeated_value);
	repeated_value.Clear();
	for (int i = 0; i < copy_value.size(); ++i)
	{
		if (copy_value.Get(i) == find_value)
		{
			remove_time += 1;
		}
		else
		{
			repeated_value.Add(copy_value.Get(i));
		}
	}

	return remove_time;
}

#endif