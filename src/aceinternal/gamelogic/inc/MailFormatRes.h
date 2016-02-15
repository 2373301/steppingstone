#ifndef MAIL_FORMAT__RES_HPP
#define MAIL_FORMAT__RES_HPP
#include <string>

struct Mail_formatInfo
{
	Mail_formatInfo()
	: addresser("")
	, id(0)
	, pay_type(0)
	, pay_value(0)
	, title("")
	, word("")
	{}
	std::string addresser;
	uint32 id;
	uint32 pay_type;
	uint32 pay_value;
	std::string title;
	std::string word;
};
#endif
