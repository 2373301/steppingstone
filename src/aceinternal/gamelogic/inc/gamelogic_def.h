
#ifndef GAMELOGIC_MACRO_HPP
#define GAMELOGIC_MACRO_HPP

#ifdef WIN32

	#ifdef GAMELOGIC_EXPORTS
	#define GAMELOGIC_EXOPRT _declspec (dllexport)
	#else
	#define GAMELOGIC_EXOPRT _declspec (dllimport)
	#endif

#else
	#define GAMELOGIC_EXOPRT 
#endif


#endif