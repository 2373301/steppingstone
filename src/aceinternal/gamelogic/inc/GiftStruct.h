#ifndef __GIFT_STRUCT_H_
#define __GIFT_STRUCT_H_

#include <vector>
#pragma warning(push, 2)
#include "typedef.h"
#pragma warning(pop)

struct GiftInfo {
    uint32 type;
    std::vector<uint32> times;
    std::vector<uint32> gifts;
};

#endif
