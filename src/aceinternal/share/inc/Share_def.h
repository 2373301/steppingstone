
#ifndef SHARE_MACRO_HPP
#define SHARE_MACRO_HPP

#ifdef WIN32

	#ifdef SHARE_EXPORTS
		#define SHARE_EXOPRT _declspec (dllexport)
	#else
		#define SHARE_EXOPRT _declspec (dllimport)
	#endif
#else
	#define SHARE_EXOPRT  
#endif

#endif
