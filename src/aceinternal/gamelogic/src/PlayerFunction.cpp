
#include <ace/Time_Value.h>
#include <ace/Date_Time.h>
#include "PlayerFunction.h"
#include "Scene.h"
#include "Resource.h"

uint64 getCurrentTimeInSec()
{
	uint64 curr_time_in_sec = 0;
	ACE_OS::gettimeofday().to_usec(curr_time_in_sec);
	curr_time_in_sec /= 1000000;

	return curr_time_in_sec;
}

uint32 getTimeDifferentDay(uint64 big_time_sec, uint64 small_time_sec)
{
	if (big_time_sec <= small_time_sec)
	{
		return 0;
	}

	ACE_Time_Value small_tv(small_time_sec, 0);
	ACE_Date_Time small_date(small_tv);
	uint64 small_zero_time = small_time_sec - small_date.hour() * 3600 - small_date.minute() * 60 - small_date.second();

	//ACE_Time_Value  big_tv(big_time_sec, 0);
	//ACE_Date_Time	big_date(big_tv);
	//uint64 big_zero_time = big_time_sec = big_date.hour() * 3600 - big_date.minute() * 60 - big_date.second();

	return (big_time_sec - small_zero_time) / (3600 * 24);
}

int getRoleAttr(typed::entity::Role * role, RoleAttrType role_attr)
{
	if ((role_attr >= RAT_ENDING) || (role_attr <= RAT_BEGIN))
	{
		// error
		return -1;
	}

	return role->attributes(role_attr);
}

//bool calculateFormBonus(Scene * scene, typed::entity::Role * role, FormResInfo * form_info, bool notify_client)
//{
//	if ((NULL == role) || (NULL == form_info))
//	{
//		return false;
//	}
//
//	for (vector<std::pair<int, int> >::iterator it = form_info->attr_increase.begin(); it != form_info->attr_increase.end(); ++it)
//	{
//		scene->modifyRoleAttrBySchemeId(role, (RoleAttrType)it->first, it->second, notify_client);
//	}
//
//	return true;
//}
//
//bool calculateFormBonus(Scene * scene, vector<typed::entity::Role *> & roles, FormResInfo * form_info, bool notify_client)
//{
//	bool result = true;
//	for (vector<typed::entity::Role *>::iterator it = roles.begin(); it != roles.end(); ++it)
//	{
//		result = calculateFormBonus(scene, *it, form_info, notify_client) && result;
//	}
//
//	return result;
//}

void fillClientItemContent(typed::entity::Item * item, typed::protocol::smsg_create_item * create_item)
{
	create_item->clear_attribute_id();
	create_item->clear_attribute_value();
	create_item->clear_gems();

	create_item->set_guid(item->guid());
	create_item->set_player(item->player());
	create_item->set_role(item->role());
	create_item->set_id(item->id());
	create_item->set_num(item->num());
	create_item->set_strength_level(item->strength_level());
	create_item->set_location(item->location());
	create_item->set_pos(item->pos());
	create_item->set_price(item->price());
	create_item->set_color(item->color());

	create_item->clear_attribute_id();
	create_item->clear_attribute_value();
	for (int i = 0; i < item->attribute_id_size(); ++i)
	{
		create_item->add_attribute_id(item->attribute_id(i));
		create_item->add_attribute_value(item->attribute_value(i));
	}
	for (int i = 0, n = item->gems_size(); i < n; ++i)
		create_item->add_gems(item->gems(i));
}

GOOGLE_MESSAGE_TYPE * cloneMessage(GOOGLE_MESSAGE_TYPE * msg)
{
	if (NULL == msg)
	{
		return NULL;
	}

	GOOGLE_MESSAGE_TYPE * clone_msg = msg->New();
	clone_msg->ParseFromString(msg->SerializeAsString());
	return clone_msg;
}

int get_UTF8_strlen(const char* src, int length)
{
	unsigned char *p;

	unsigned char *pStrEnd;
	unsigned char *pCharEnd;
	int bytes = 0;
	unsigned char *pInvalidCharStart;

	pStrEnd = (unsigned char *)src + length;
	int len = 0;
	p = (unsigned char *)src;
	pInvalidCharStart = NULL;
	while (p < pStrEnd)
	{
		if (*p < 0x80)
		{
			p++;
			len++;
			continue;
		}

		if ((*p & 0xE0) == 0xC0)  //110xxxxx
		{
			bytes = 1;
			len++;
		}
		else if ((*p & 0xF0) == 0xE0) //1110xxxx
		{
			bytes = 2;
			len++;
		}
		else if ((*p & 0xF8) == 0xF0) //11110xxx
		{
			bytes = 3;
			len++;
		}
		else if ((*p & 0xFC) == 0xF8) //111110xx

		{
			bytes = 4;
			len++;
		}
		else if ((*p & 0xFE) == 0xFC) //1111110x
		{
			bytes = 5;
			len++;
		}
		else
		{
			pInvalidCharStart = p;
			break;
		}

		p++;
		pCharEnd = p + bytes;

		for (; p<pCharEnd; p++)
		{
			if ((*p & 0xC0) != 0x80)
			{
				break;
			}
		}
	}
	return len;
}

void fillRoleAttrRank(::google::protobuf::RepeatedField< ::google::protobuf::uint32 > * attributes, typed::entity::Role * role, typed::entity::Player * player, SystemResInfo * system_res_info)
{
	attributes->Add(role->level());
	attributes->Add(role->attributes(RAT_HP));
	attributes->Add(role->attributes(RAT_STR_ATTACK));
	attributes->Add(role->attributes(RAT_STR_DEFENSE));
	attributes->Add(role->attributes(RAT_AGI_ATTACK));
	attributes->Add(role->attributes(RAT_AGI_DEFENSE));
	attributes->Add(role->attributes(RAT_INT_ATTACK));
	attributes->Add(role->attributes(RAT_INT_DEFENSE));
	attributes->Add(getAttrRank(role, system_res_info, RAT_HIT, RAT_HIT_RANK, player->level()));
	attributes->Add(getAttrRank(role, system_res_info, RAT_DODGE, RAT_DODGE_RANK, player->level()));
	attributes->Add(getAttrRank(role, system_res_info, RAT_CRITICAL, RAT_CRITICAL_RANK, player->level()));
	attributes->Add(getAttrRank(role, system_res_info, RAT_BLOCK, RAT_BLOCK_RANK, player->level()));
	attributes->Add(getAttrRank(role, system_res_info, RAT_MULTI_HIT, RAT_MULTI_HIT_RANK, player->level()));
	attributes->Add(getAttrRank(role, system_res_info, RAT_UNION_HIT, RAT_UNION_HIT_RANK, player->level()));

	attributes->Add(role->attributes(RAT_INITIATIVE_ALL));
	attributes->Add(role->attributes(RAT_INITIATIVE_COLOR));
	attributes->Add(role->attributes(RAT_INITIATIVE_STHEN));
	attributes->Add(role->attributes(RAT_INITIATIVE_LAW));
	attributes->Add(role->attributes(RAT_INITIATIVE_EDUCATE));
	attributes->Add(role->attributes(RAT_INITIATIVE_DODGE));
	attributes->Add(role->attributes(RAT_INITIATIVE_CHAKRE));
	attributes->Add(role->attributes(RAT_INITIATIVE_TRAINING));
	attributes->Add(role->attributes(RAT_INITIATIVE_LEVEL));

	attributes->Add(getAttrRank(role, system_res_info, RAT_UNCRITICAL, RAT_UNCRITICAL_RANK, player->level()));
	attributes->Add(getAttrRank(role, system_res_info, RAT_UNBLOCK, RAT_UNBLOCK_RANK, player->level()));

}

#define CONVERT_RANK(attr_value, player_level)	attr_value * system_res_info->rank_param / player_level

int getAttrRank(typed::entity::Role * role, SystemResInfo * system_res_info, int attr_index, int attr_rank_index, int player_level)
{
	int rank_value = role->attributes(attr_rank_index);
	int rank_attr = CONVERT_RANK(rank_value, player_level);
	int attr_value = role->attributes(attr_index);
	int src_attr_value = attr_value - rank_attr;
	if (src_attr_value < 0)
	{
		DEF_LOG_ERROR("get wrong attr rank value, player is <%llu>, role is <%llu>, attr index is <%d>, rank index is <%d>\n", 
			role->player(), role->guid(), attr_index, attr_rank_index);
		src_attr_value = 0;
	}
	rank_value += src_attr_value * 0.5 + 0.99;
	return rank_value;
}