
#include <boost/filesystem.hpp>
#include "Redirect.h"

#ifndef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

Redirect::Redirect()
: m_output_fd(0)
, m_input_fd(0)
, m_error_fd(0)
{
#ifndef WIN32
	boost::filesystem::path path("logs");
	if (!boost::filesystem::exists(path))
	{
		boost::filesystem::create_directory(path);
	}

	m_output_fd = open("./logs/output.txt", O_WRONLY | O_CREAT, 0600);

	m_error_fd = open("./logs/error.txt", O_WRONLY | O_CREAT, 0600);

	m_input_fd = open("./logs/input.txt", O_RDONLY | O_CREAT, 0600);

	close(0);
	close(1);
	close(2);

	dup2(m_input_fd, 0);
	dup2(m_output_fd, 1);
	dup2(m_error_fd, 2);
#endif
}

Redirect::~Redirect()
{
#ifndef WIN32
	close(m_output_fd);
	close(m_input_fd);
	close(m_error_fd);
#endif
}
