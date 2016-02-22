#ifndef Scenex_def_h__
#define Scenex_def_h__


#ifdef WIN32

#ifdef SCENEX_EXPORTS
#define SCENEX_EXOPRT _declspec (dllexport)
#else
#define SCENEX_EXOPRT _declspec (dllimport)
#endif

#else
#define SCENEX_EXOPRT 
#endif


#endif // Scenex_def_h__
