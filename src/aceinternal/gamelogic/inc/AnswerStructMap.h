#ifndef __ANSWER_STRUCT_MAP_H_
#define __ANSWER_STRUCT_MAP_H_

#pragma warning(push, 2)
#include "typedef.h"
#pragma warning(pop)

#include <vector>
#include <boost/unordered_map.hpp>

struct AnswerEvent;
struct AnswerSection;
struct AnswerQuestion;

struct AnswerResource {
    uint32 etotal;
    std::vector<AnswerEvent*> events;
    std::vector<AnswerSection*> sections;   // sequenced and ordered by section id.
    boost::unordered_map<uint32, AnswerQuestion*> questions;

    uint32 power_per_cd;
    uint32 power_top_cd;

    uint32 refresh_coins;
    uint32 power_coins;
    uint32 power_unit;
};

#endif
