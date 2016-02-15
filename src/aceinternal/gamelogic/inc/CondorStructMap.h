#ifndef __CONDOR_STRUCT_MAP_H
#define __CONDOR_STRUCT_MAP_H

#pragma warning(push, 2)
#include "typedef.h"
#pragma warning(pop)

struct CondorPrey;
struct CondorWeapon;
struct CondorFormula;

#include <boost/unordered_map.hpp>
typedef boost::unordered_map<uint32, CondorPrey *> CondorPreyMap;
typedef boost::unordered_map<uint32, CondorWeapon *> CondorWeaponMap;

#endif
