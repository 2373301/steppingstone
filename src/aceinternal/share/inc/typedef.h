
#ifndef APSS_TYPEDEF_HPP
#define APSS_TYPEDEF_HPP

#include <boost/cstdint.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/lexical_cast.hpp>

typedef boost::int8_t	int8;
typedef boost::int16_t	int16;
typedef boost::int32_t	int32;
typedef boost::int64_t	int64;

typedef boost::uint8_t	uint8;
typedef boost::uint16_t	uint16;
typedef boost::uint32_t	uint32;
typedef boost::uint64_t	uint64;

#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <iostream>
#include <memory>
#include <set>

#define define_unordered_map std::map

#define define_unordered_set std::set

using namespace std;

typedef vector<uint32>	Uint32Vec_t;
typedef vector<int32>	Int32Vec_t;
typedef vector<uint64>	Uint64Vec_t;

typedef vector<string>	StringVec_t;

typedef define_unordered_set<uint32>		Uint32Set_t;

typedef define_unordered_set<uint64>		Uint64Set_t;

#endif
