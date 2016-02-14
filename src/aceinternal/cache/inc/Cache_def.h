
#ifndef CACHE_MACRO_HPP
#define CACHE_MACRO_HPP

#ifdef WIN32

	#ifdef CACHE_EXPORTS
	#define CACHE_EXOPRT _declspec (dllexport)
	#else
	#define CACHE_EXOPRT _declspec (dllimport)
	#endif

#else
	#define CACHE_EXOPRT  
#endif

#endif
