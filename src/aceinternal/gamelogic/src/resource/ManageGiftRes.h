#ifndef __GIFT_RES_LOADER_H_
#define __GIFT_RES_LOADER_H_

#include "GiftStruct.h"
#include "GiftStructR.h"

#include <boost/ptr_container/ptr_map.hpp>

class Document;
class Element;

class ManageGiftRes {
public:
    bool Load(Document *doc);

    GiftResource const& getResource();

private:
    bool readGift(Element *ele);

    GiftResource res;

    typedef ::boost::ptr_map<uint32, GiftInfo> GiftContainer;
    GiftContainer gift_container;
};

#endif
