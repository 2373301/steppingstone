
#ifndef TERMINAL_MACRO_HPP
#define TERMINAL_MACRO_HPP

#ifdef WIN32

	#ifdef TERMINAL_EXPORTS
	#define TERMINAL_EXOPRT _declspec (dllexport)
	#else
	#define TERMINAL_EXOPRT _declspec (dllimport)
	#endif

#else
	#define TERMINAL_EXOPRT 
#endif

#endif