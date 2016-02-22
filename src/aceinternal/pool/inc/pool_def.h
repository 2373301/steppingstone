
#ifndef SCENE_MACRO_HPP
#define SCENE_MACRO_HPP

#ifdef WIN32

	#ifdef POOL_EXOPRTS
	#define POOL_EXOPRT _declspec (dllexport)
	#else
	#define POOL_EXOPRT _declspec (dllimport)
	#endif

#else
	#define POOL_EXOPRT 
#endif

#endif