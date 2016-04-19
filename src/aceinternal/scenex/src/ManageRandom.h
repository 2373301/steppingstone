
#ifndef MANAGE_RANDOM_HPP
#define MANAGE_RANDOM_HPP

#include <boost/random.hpp>

class ManageRandom
{
public:
	ManageRandom();
public:
	int IScene_getRandom(int max_no, int min_no = 0);
protected:
private:
	boost::hellekalek1995 m_rand_engineer;

	boost::uniform_int<>  m_rand_distribution;

	boost::variate_generator<boost::hellekalek1995, boost::uniform_int<> > m_rand_generator;
};
#endif