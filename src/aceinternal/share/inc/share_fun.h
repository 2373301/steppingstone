
#ifndef SHARE_FUN_HPP
#define SHARE_FUN_HPP

#include <boost/typeof/typeof.hpp>
#include "typedef.h"
#include "Share_def.h"

#define DEL_POINTER(p) if(p!=NULL){ delete p; p=NULL; }

void SHARE_EXOPRT searchFileByExt(const string & search_dir, const string & file_ext, vector<string> & searched_file);

#define cleanVectorContent(vec)	\
{	\
	BOOST_AUTO(it_, vec.begin());	\
	for ( ; it_ != vec.end(); ++it_)	\
	{	\
		DEL_POINTER(*it_);	\
	}	\
}

#define cleanMapSecondContent(map_)	\
{	\
	BOOST_AUTO(it_, map_.begin());	\
	for ( ; it_ != map_.end(); ++it_)	\
	{	\
		DEL_POINTER(it_->second);	\
	}	\
}
#endif