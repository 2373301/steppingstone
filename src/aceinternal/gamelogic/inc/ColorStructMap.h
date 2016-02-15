#ifndef __COLOR_STRUCT_MAP_H_
#define __COLOR_STRUCT_MAP_H_

struct ColorInfo;

#include <boost/unordered_map.hpp>

typedef boost::unordered_map<uint32, ColorInfo*> ColorInfoMap;

#endif
