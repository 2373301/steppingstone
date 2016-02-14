
#ifndef NETSTREAM_MACRO_HPP
#define NETSTREAM_MACRO_HPP

#ifdef WIN32

#ifdef NETSTREAM_EXPORTS
#define NETSTREAM_EXOPRT _declspec (dllexport)
#else
#define NETSTREAM_EXOPRT _declspec (dllimport)
#endif

#else
#define NETSTREAM_EXOPRT  
#endif

#endif