
#ifndef CHECK_ROLE_NAME_HPP
#define CHECK_ROLE_NAME_HPP

#include <list>
#include <map>
#include "SingletonInit.h"
#include "Singleton.h"

struct RoleNameStrategy
{
	RoleNameStrategy()
	: min_name_length(4)
	, max_name_length(12)
	{}
	typedef std::list<std::string>	StringList;

	int			min_name_length;
	int			max_name_length;
	StringList	not_allowed_word_list;
};

struct UnfawfulWordInfo
{
	UnfawfulWordInfo()
	: end(false)
	{}
	
	typedef std::map<char, UnfawfulWordInfo *> UnfawfulWordInfoMap_t;

	bool end;

	UnfawfulWordInfoMap_t children;
};

class CheckRoleName  
{
public:
	CheckRoleName();
	~CheckRoleName();
public:
	static CheckRoleName * instance()
	{
		return Singleton<CheckRoleName>::instance();
	}
public:
	/// init
	virtual int init();

	/// fini, stop
	virtual int fini();

	/// wait for finish
	virtual int wait();
public:
	bool checkRoleName(const string & role_name, int & error_number);

protected:
	bool loadUnlawfulWord();

	void processUnlawfulWord();

	bool isUnlawfulWord(const string & role_name);

	bool isUnlawfulWord(const char * check_str, int str_size);

	bool includeUnlawfulLetter(const string & role_name);

	int get_UTF8_strlen(const char* src, int str_len);
private:
	RoleNameStrategy m_role_name_strategy;

	UnfawfulWordInfo m_unfawful_world_info;
};
#endif