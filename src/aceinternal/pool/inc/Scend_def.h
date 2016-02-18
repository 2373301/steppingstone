
#ifndef SCENE_MACRO_HPP
#define SCENE_MACRO_HPP

#ifdef WIN32

	#ifdef SCENE_EXPORTS
	#define SCENE_EXOPRT _declspec (dllexport)
	#else
	#define SCENE_EXOPRT _declspec (dllimport)
	#endif

#else
	#define SCENE_EXOPRT 
#endif

#endif