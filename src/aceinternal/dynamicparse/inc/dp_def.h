#ifndef dp_def_h__
#define dp_def_h__

#ifdef WIN32

#ifdef DYNAMIC_PARSE_EXPORTS
#define DYNAMIC_PARSE_EXPORT _declspec (dllexport)
#else
#define DYNAMIC_PARSE_EXPORT _declspec (dllimport)
#endif

#else
#define DYNAMIC_PARSE_EXPORT  
#endif

#endif // dp_def_h__
