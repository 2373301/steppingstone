#include "ManageGiftRes.h"

#pragma warning(push, 2)
#include "SimpleXML.h"
#pragma warning(pop)

#include <boost/foreach.hpp>
#include <boost/typeof/typeof.hpp>

#define nullptr NULL

#define DIRTY_COMMON()              \
Element *root = nullptr;            \
do {                                \
    if (doc == nullptr) {           \
        return false;               \
    }                               \
                                    \
    root = doc->get_root();         \
    if (root == nullptr) {          \
        return false;               \
    }                               \
} while(0)

GiftResource const&
ManageGiftRes::getResource() {
    return res;
}

enum {
    kGiftTypeTime       = 1,
    kGiftTypeLong       = 2,
    kGiftTypeN          = 2,
};

bool
ManageGiftRes::Load(Document *doc) {
    DIRTY_COMMON();

    BOOST_FOREACH(Element *ele, root->get_elements()) {
        if (!readGift(ele)) {
            return false;
        }
    }
    res.gifts.resize(kGiftTypeN);
    BOOST_FOREACH(GiftContainer::value_type v, gift_container) {
        if (v.first > kGiftTypeN) {
            return false;
        }
        res.gifts[v.first-1] = v.second;
    }
    BOOST_FOREACH(GiftInfo *info, res.gifts) {
        if (info == nullptr) return false;
    }
    return true;
}

enum { kMinute = 60 * 1000 };

bool
ManageGiftRes::readGift(Element *ele) {
    bool succ = false;
    do {
        struct GiftInfo info;
        if (!(getAttrValue(ele, "type", info.type)
              && getAttrValue(ele, "time", info.times)
              && getAttrValue(ele, "gifts", info.gifts))) {
            break;
        }
        if (info.type == kGiftTypeTime) {
            if (info.times.size() != info.gifts.size()) {
                break;
            }
            uint32 prev = 0;
            for (size_t i=0,n=info.times.size(); i<n; ++i) {
                uint32 curr = info.times[i];
                if (curr < prev) return false;
                info.times[i] = kMinute * curr;
            }
        }

        gift_container[info.type] = info;
        succ = true;
    } while (0);
    return succ;
}
