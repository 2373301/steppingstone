
#include <boost/algorithm/string.hpp>
#include "SimpleXML.h"
#include "CheckRoleName.h"
#include "msg_error.pb.h"
#include "Logger.h"
#include "ManageConfig.h"

CheckRoleName::CheckRoleName()
{

}

CheckRoleName::~CheckRoleName()
{

}

int CheckRoleName::init()
{
	if (loadUnlawfulWord())
	{
		processUnlawfulWord();

		return 0;
	}
	else
	{
		return -1;
	}
}

int CheckRoleName::fini()
{
	return 0;
}

int CheckRoleName::wait()
{
	return 0;
}

bool CheckRoleName::checkRoleName(const string & role_name, int & error_number)
{
	int name_len = get_UTF8_strlen(role_name.c_str(), role_name.length());

	string check_str = role_name;
	boost::to_lower(check_str);
	if (name_len < m_role_name_strategy.min_name_length)
	{
		//error_number = typed::protocol::smsg_error_ecode_EFAILED_CHECK_ROLE_NAME;
		return false;
	}

	if (name_len > m_role_name_strategy.max_name_length)
	{
		//error_number = typed::protocol::smsg_error_ecode_EFAILED_CHECK_ROLE_NAME;
		return false;
	}

	if (includeUnlawfulLetter(check_str))
	{
		//error_number = typed::protocol::smsg_error_ecode_EFAILED_CHECK_ROLE_NAME;
		return false;
	}

	/// check unlawful words
	if (isUnlawfulWord(check_str))
	{
		//error_number = typed::protocol::smsg_error_ecode_EFAILED_CHECK_ROLE_NAME;
		return false;
	}
	return true;
}

bool CheckRoleName::loadUnlawfulWord()
{
	bool result = false;
	std::auto_ptr<Document> auto_doc(XMLFactory::create_document());
	if (NULL == auto_doc.get())
	{
		DEF_LOG_ERROR("Failed to call XMLFactory::create_document()\n");
		return false;
	}

	try
	{
		if (auto_doc->load_file(ManageConfig::instance()->getUnfawfulFile()))
		{
			result = true;
			Element * root_ele = auto_doc->get_root();
			

			if (NULL == root_ele)
			{
				DEF_LOG_ERROR("The root element is NULL\n");
				return false;
			}
			Element * account_ele = root_ele->get_element("Account");
			if (NULL == account_ele)
			{
				DEF_LOG_ERROR("Failed to get Account element\n");
				return false;
			}

			Elements world_ele_list = account_ele->get_elements();

			for (Elements::iterator it = world_ele_list.begin(); it != world_ele_list.end(); ++it)
			{
				Element * world_ele = *it;
				string str_value = world_ele->get_text();
				boost::to_lower(str_value);
				m_role_name_strategy.not_allowed_word_list.push_back(str_value);
			}
		}
		else
		{
			result = false;
			DEF_LOG_ERROR("Failed to parse file : <%s>\n", ManageConfig::instance()->getUnfawfulFile().c_str());
		}
	}
	catch (...)
	{
		DEF_LOG_ERROR("Raise unknown exception in CheckRoleName::loadConfig, the last error is <%d>\n", ACE_OS::last_error());
		result = false;
	}
	auto_doc->clear();
	return result;
}

void CheckRoleName::processUnlawfulWord()
{
	for (RoleNameStrategy::StringList::iterator it = m_role_name_strategy.not_allowed_word_list.begin(); it != m_role_name_strategy.not_allowed_word_list.end(); ++it)
	{
		const char * src_str = it->c_str();
		size_t str_size = it->size();
		if (0 == str_size)
		{
			continue;
		}

		UnfawfulWordInfo::UnfawfulWordInfoMap_t * current_unfawful_word_info = &m_unfawful_world_info.children;
		char current_byte = 0;
		for (int i = 0; i < str_size; ++i)
		{
			current_byte = src_str[i];
			UnfawfulWordInfo::UnfawfulWordInfoMap_t::iterator it = current_unfawful_word_info->find(current_byte);
			if (it == current_unfawful_word_info->end())
			{
				UnfawfulWordInfo * world_info = new UnfawfulWordInfo();
				if (str_size - 1 == i)
				{
					// leaf
					world_info->end = true;
				}
				(*current_unfawful_word_info)[current_byte] = world_info;
				current_unfawful_word_info = &world_info->children;
			}
			else
			{
				current_unfawful_word_info = &it->second->children;
			}
		}
	}
}

bool CheckRoleName::isUnlawfulWord(const string & role_name)
{
	bool result = false;
	const char * src = role_name.c_str();
	for (int i = 0; i < role_name.size(); ++i)
	{
		if (isUnlawfulWord(src + i, role_name.size() - i))
		{
			result = true;
			break;
		}
	}
	return result;
}

bool CheckRoleName::isUnlawfulWord(const char * check_str, int str_size)
{
	bool result = true;
	UnfawfulWordInfo::UnfawfulWordInfoMap_t * current_unfawful_word_info = &m_unfawful_world_info.children;
	for (int i = 0; i < str_size; ++i)
	{
		char current_char = check_str[i];
		UnfawfulWordInfo::UnfawfulWordInfoMap_t::iterator it = current_unfawful_word_info->find(current_char);
		if (it != current_unfawful_word_info->end())
		{
			if (it->second->end)
			{
				// include word, is unlawful
				result = true;
				break;
			}
			else
			{
				current_unfawful_word_info = &it->second->children;
				result = false;
			}
		}
		else
		{
			result = false;
			break;
		}
	}

	return result;
}

bool CheckRoleName::includeUnlawfulLetter(const string & role_name)
{
	bool result = false;
	for (int i = 0; i < role_name.size(); ++i)
	{
		unsigned char c = role_name[i];
		if (c < 0x80)
		{
			if (((c >= 'a') && (c <= 'z'))
				|| ((c >= '0') && (c <= '9'))
				)
			{
				result = false;
			}
			else
			{
				result = true;
				break;
			}
		}
		else
		{
			result = false;
		}
	}

	return result;
}

int CheckRoleName::get_UTF8_strlen(const char* src, int str_len)
{
	int len = 0;
	unsigned char *p;
	unsigned char *pStrEnd;
	unsigned char *pCharEnd;
	int bytes;
	unsigned char *pInvalidCharStart;

	pStrEnd = (unsigned char *)src + str_len;
	len = 0;
	p = (unsigned char *)src;
	pInvalidCharStart = NULL;
	int char_size = 0;
	int chinese_size = 0;
	while (p < pStrEnd)
	{
		if (*p < 0x80)
		{
			p++;
			char_size++;
			continue;
		}

		if ((*p & 0xE0) == 0xC0)  //110xxxxx
		{
			bytes = 1;
			chinese_size++;
		}
		else if ((*p & 0xF0) == 0xE0) //1110xxxx
		{
			bytes = 2;
			chinese_size++;
		}
		else if ((*p & 0xF8) == 0xF0) //11110xxx
		{
			bytes = 3;
			chinese_size++;
		}
		else if ((*p & 0xFC) == 0xF8) //111110xx
		{
			bytes = 4;
			chinese_size++;
		}
		else if ((*p & 0xFE) == 0xFC) //1111110x
		{
			bytes = 5;
			chinese_size++;
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

	len = char_size + chinese_size * 2;

	return len;
}