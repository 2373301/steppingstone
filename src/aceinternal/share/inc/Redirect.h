
#ifndef REDIRECT_HPP
#define REDIRECT_HPP

#define START_DAEMONIZE_CMD()	\
	ACE::daemonize("./");	\
	Redirect * redirect = new Redirect();

#define IGNORE_LINUX_SINGLE()	\
	signal(SIGQUIT, SIG_IGN);	\
	signal(SIGPIPE, SIG_IGN);


class Redirect
{
public:
	Redirect();
	~Redirect();
protected:
private:
	int m_output_fd;
	int m_input_fd;
	int m_error_fd;
};

#endif
