
#ifndef MEMBER_PROTOCOL_HPP
#define MEMBER_PROTOCOL_HPP

#include "typedef.h"

#define MEMBER_ACCOUNT_LENGTH			32

#define	GATE_ADDR_LENGTH				32

#define MEMBER_SECURITY_KEY_LENGTH		32

#define ROLE_NAME_LENGTH				32

#define MEMBER_GUID_LENGTH				24

#define DATE_LENGTH                     10

#define QQ_INFO_LENGTH					32

void copyAccount(char * dst_account, const char * src_account);

void copyGateAddr(char * dst_gate_addr, const char * src_gate_addr);

void copySecurityKey(char * dst_security, const char * src_security);

void copyRoleName(char * dst_name, const char * src_name);

void copyRoleGuid(char * dst_guid, const char * src_guid);

namespace membersys
{

#pragma pack(1)

struct mmsg_member_register
{
	mmsg_member_register()
		: line_id(0)
	{
		memset(security_key, 0, MEMBER_SECURITY_KEY_LENGTH);
	}

	uint32	line_id;
	char	security_key[MEMBER_SECURITY_KEY_LENGTH];
};

struct mmsg_member_login
{
	mmsg_member_login()
		: request_index(0)
		, addiction_flag(0)
		, network_type(0)
		, line_id(0)
	{
		memset(account, 0, MEMBER_ACCOUNT_LENGTH);
		memset(security_key, 0, MEMBER_SECURITY_KEY_LENGTH);

		memset(open_id, 0, QQ_INFO_LENGTH);
		memset(open_key, 0, QQ_INFO_LENGTH);
		memset(pf, 0, QQ_INFO_LENGTH);
		memset(pfkey, 0, QQ_INFO_LENGTH);
	}

	uint32	request_index;								// 标示每个请求的唯一索引
	char	account[MEMBER_ACCOUNT_LENGTH];				// char类型，长度32
	uint32	addiction_flag;								// 防沉迷标志，1：不是防沉迷用户，2:是防沉迷用户
	uint32	network_type;								// 玩家网络类型，1：电信，2：网通
	char	security_key[MEMBER_SECURITY_KEY_LENGTH];	// char类型，长度32
	uint32	line_id;

	char	open_id[QQ_INFO_LENGTH];	// qq open_id
	char	open_key[QQ_INFO_LENGTH];	// qq session key
	char	pf[QQ_INFO_LENGTH];			// qq 平台id
	char	pfkey[QQ_INFO_LENGTH];		// qq 平台key
};

struct smsg_member_login_respond 
{
	smsg_member_login_respond()
		: request_index(0)
	{
		memset(account, 0, MEMBER_ACCOUNT_LENGTH);
		memset(gate_addr, 0, GATE_ADDR_LENGTH);
	}

	uint32 request_index;
	char   account[MEMBER_ACCOUNT_LENGTH];
	char   gate_addr[GATE_ADDR_LENGTH];
};

struct smsg_member_login_confirm 
{
	smsg_member_login_confirm()
		: request_index(0)
	{
		memset(account, 0, MEMBER_ACCOUNT_LENGTH);
	}

	uint32 request_index;
	char   account[MEMBER_ACCOUNT_LENGTH];
};

//////////////////////////////////////////////////////////////////////////

struct mmsg_member_charge 
{
	mmsg_member_charge()
		: request_index(0)
		, point(0)
		, line_id(0)
	{
		memset(account, 0, MEMBER_ACCOUNT_LENGTH);
		memset(role_guid, 0, MEMBER_GUID_LENGTH);
	}

	uint32	request_index;							// 标示每个请求的唯一索引
	char	account[MEMBER_ACCOUNT_LENGTH];			// char类型，长度32
	uint32	point;									// 充值元宝数
	uint32  line_id;
	char	role_guid[MEMBER_GUID_LENGTH];			// 合服之后，如果一条线有多个角色，需要带上角色guid，默认填0
};

struct smsg_member_charge_confirm 
{
	smsg_member_charge_confirm()
		: request_index(0)
		, result(0)
	{
		memset(account, 0, MEMBER_ACCOUNT_LENGTH);
	}

	uint32 request_index;
	char   account[MEMBER_ACCOUNT_LENGTH];
	uint32 result;
};

struct mmsg_query_role
{
	mmsg_query_role()
		: request_index(0)
		, line_id(0)
	{
		memset(account, 0, MEMBER_ACCOUNT_LENGTH);
	}

	uint32 request_index;
	char   account[MEMBER_ACCOUNT_LENGTH];			// char类型，长度32
	uint32 line_id;									// 玩家选择的线ID
};

// 每日最高在线
// 每日平均在线
struct mmsg_statistics_players
{
    mmsg_statistics_players()
        :request_index(0)
        ,line_id(0)
    {
        memset(date, 0, DATE_LENGTH);
    }

    uint32 request_index;
    char   date[DATE_LENGTH]; // 20xx-xx-xx
    uint32 line_id;
};

//批量查询某天注册用户的相关信息
struct mmsg_query_register_players
{
    mmsg_query_register_players()
        :request_index(0)
        ,numPerPage(0)
        ,current_page(0)
        ,line_id(0)
    {
        memset(date, 0, DATE_LENGTH);
    }

    uint32 request_index;
    char date[DATE_LENGTH]; // 20xx-xx-xx
    uint32 numPerPage;
    uint32 current_page;
    uint32 line_id;
};

// 查询平均在线玩家数, 最高在线玩家数
struct mmsg_query_daily_players
{
    mmsg_query_daily_players()
        :request_index(0)
        ,line_id(0)
    {
        memset(date, 0, DATE_LENGTH);
    }

    uint32 request_index;
    char date[DATE_LENGTH]; // 20xx-xx-xx
    uint32 line_id;
};



struct RoleInfo
{
	RoleInfo()
	: role_level(0)
	, line_id(0)
	{
		memset(role_name, 0, ROLE_NAME_LENGTH);
		memset(role_id, 0, MEMBER_GUID_LENGTH);
	}
	char role_id[MEMBER_GUID_LENGTH];
	uint32 role_level;
	char   role_name[ROLE_NAME_LENGTH];		// 长度32
	uint32 line_id;
};


struct smsg_query_role_return
{
	smsg_query_role_return()
	: request_index(0)
	, line_id(0)
	, query_status(0)
	{
		memset(account, 0, MEMBER_ACCOUNT_LENGTH);
	}
	uint32 request_index;
	char   account[MEMBER_ACCOUNT_LENGTH];	// 长度32
	uint32 line_id;
	uint32 query_status;		// 1 : success, 2:failed
	//RoleInfo role_info;	// 对于数组传输格式：16位的数组长度，后面依次填充每个数据的内容。
	// 如果长度为0，则只有一个16位的长度0，后面无填充内容。
};

struct mmsg_query_online_number 
{
	mmsg_query_online_number()
	: request_index(0)
	, line_id(0)
	{}

	uint32 request_index;
	uint32 line_id;
};

struct smsg_query_online_number 
{
	smsg_query_online_number()
	: request_index(0)
	, line_id(0)
	, online_number(0)
	{}

	uint32 request_index;
	uint32 line_id;
	uint32 online_number;
};

struct mmsg_member_give_gift
{
	mmsg_member_give_gift()
	: request_index(0)
	, line_id(0)	
	, gift_number(0)
	, replay(0)	
	, gift_type(0)
	, start_day(0)
	, end_day(0)

	{
		memset(account, 0, MEMBER_ACCOUNT_LENGTH);
		memset(role_guid, 0, MEMBER_GUID_LENGTH);
	}

	uint32	request_index;
	uint32 line_id;
	char   account[MEMBER_ACCOUNT_LENGTH];	// 长度32
	uint32 gift_number;
	uint32 replay;
	uint32 gift_type;
	uint32 start_day;
	uint32 end_day;
	char   role_guid[MEMBER_GUID_LENGTH];			// 合服之后，如果一条线有多个角色，需要带上角色guid，默认填0
};

struct smsg_member_give_gift 
{
	smsg_member_give_gift()
		: request_index(0)
		, line_id(0)
		, result(0)
	{
		memset(account, 0, MEMBER_ACCOUNT_LENGTH);
		memset(role_guid, 0, MEMBER_GUID_LENGTH);
	}

	uint32 request_index;
	uint32 line_id;
	char account[MEMBER_ACCOUNT_LENGTH];
	char role_guid[MEMBER_GUID_LENGTH];			// 合服之后，如果一条线有多个角色，需要带上角色guid，默认填0
	uint32 result;
};


#pragma pack(8)

}

#endif
