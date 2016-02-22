
#include "ManageRandom.h"
#include "Logger.h"

ManageRandom::ManageRandom()
: m_rand_engineer(::time(NULL))
, m_rand_distribution(0, 100000000)
, m_rand_generator(m_rand_engineer, m_rand_distribution)
{}

int ManageRandom::get_random(int max_no, int min_no)
{
	if (max_no < min_no)
	{
		DEF_LOG_ERROR("error param, max_no is <%d>, min_no si <%d>\n", max_no, min_no);
		return min_no;
	}

	int v = m_rand_generator();
	int result = (v % (max_no - min_no + 1)) + min_no;

	DEF_LOG_DEBUG("get random number is <%d>, min is <%d>, max is <%d>, result is <%d>\n", v, min_no, max_no, result);

	return result;
}
