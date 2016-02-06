
#ifndef LOGGER_DEF_HPP
#define LOGGER_DEF_HPP

#ifdef WIN32

#ifdef LOGGER_EXPORTS
	#define LOGGER_EXOPRT _declspec (dllexport)
#else
	#define LOGGER_EXOPRT _declspec (dllimport)
#endif

#else
	#define LOGGER_EXOPRT 
#endif

#endif