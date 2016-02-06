
#ifndef NETCORE_MACRO_HPP
#define NETCORE_MACRO_HPP

#ifdef WIN32

#ifdef NETCORE_EXPORTS
#define NETCORE_EXOPRT _declspec (dllexport)
#else
#define NETCORE_EXOPRT _declspec (dllimport)
#endif

#else
	#define NETCORE_EXOPRT  
#endif

#endif