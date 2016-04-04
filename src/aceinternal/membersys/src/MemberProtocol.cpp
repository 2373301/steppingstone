
#include "MemberProtocol.h"

void copyAccount(char * dst_account, const char * src_account)
{
	int account_len = strlen(src_account);
	if (account_len > MEMBER_ACCOUNT_LENGTH)
	{
		account_len = MEMBER_ACCOUNT_LENGTH;
	}

	memcpy(dst_account, src_account, account_len);
}

void copyGateAddr(char * dst_gate_addr, const char * src_gate_addr)
{
	int str_len = strlen(src_gate_addr);
	if (str_len > GATE_ADDR_LENGTH)
	{
		str_len = GATE_ADDR_LENGTH;
	}

	memcpy(dst_gate_addr, src_gate_addr, str_len);
}

void copySecurityKey(char * dst_security, const char * src_security)
{
	int security_len = strlen(src_security);
	if (security_len > MEMBER_SECURITY_KEY_LENGTH)
	{
		security_len = MEMBER_SECURITY_KEY_LENGTH;
	}

	memcpy(dst_security, src_security, security_len);
}

void copyRoleName(char * dst_name, const char * src_name)
{
	int name_len = strlen(src_name);
	if (name_len > ROLE_NAME_LENGTH)
	{
		name_len = ROLE_NAME_LENGTH;
	}

	memcpy(dst_name, src_name, name_len);
}

void copyRoleGuid(char * dst_guid, const char * src_guid)
{
	int guid_len = strlen(src_guid);
	if (guid_len > MEMBER_GUID_LENGTH)
	{
		guid_len = MEMBER_GUID_LENGTH;
	}

	memcpy(dst_guid, src_guid, guid_len);
}
