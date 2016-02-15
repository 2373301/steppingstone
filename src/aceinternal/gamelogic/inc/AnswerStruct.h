#ifndef __ANSWER_STRUCT_H_
#define __ANSWER_STRUCT_H_

#pragma warning(push, 2)
#include "typedef.h"
#pragma warning(pop)

#include <vector>

struct AnswerOption {
    std::vector<uint32> zids;
    std::vector<uint32> argas;
    std::vector<uint32> argbs;
};

struct AnswerQuestion {
    uint32 id;
    uint32 section;
    std::vector<AnswerOption> options;
};

struct AnswerSection {
    uint32 id;
    std::vector<AnswerQuestion*> questions;
};

struct AnswerEvent {
    uint32 id;
    uint32 zid;
    uint32 arga;
    uint32 argb;
    uint32 weight;
};

#endif
