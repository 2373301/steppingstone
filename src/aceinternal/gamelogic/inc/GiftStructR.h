#ifndef __GIFT_STRUCT_MAP_H_
#define __GIFT_STRUCT_MAP_H_

#include <vector>

struct GiftInfo;
struct GiftResource {
    std::vector<GiftInfo*> gifts;
};

#endif
