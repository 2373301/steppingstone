#ifndef MAIL__RES_HPP
#define MAIL__RES_HPP
struct MailInfo
{
	MailInfo()
	: attachment(0)
	, id(0)
	, mail_survival(0)
	, mailbox_limit(0)
	, mass_lilmit(0)
	, pay_mail(0)
	, title_limit(0)
	, word_limit(0)
	{}
	uint32 attachment;
	uint32 id;
	uint32 mail_survival;
	uint32 mailbox_limit;
	uint32 mass_lilmit;
	uint32 pay_mail;
	uint32 title_limit;
	uint32 word_limit;
};
#endif
