#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#pragma warning(push, 2)
#include "SimpleXML.h"
#include "gamelogic_def.h"
#pragma warning(pop)

#include "ManageIllusionRes.h"

#include <cstdio>
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

#pragma warning(push)
#pragma warning(disable : 4251 4996)

ColorInfoMap const&
ManageIllusionRes::getColorInfoMap() {
    return color_map;
}

bool
ManageIllusionRes::Load(Document *doc) {
    return loadColorInfo(doc);
}

bool
ManageIllusionRes::loadColorInfo(Document *doc) {
    DIRTY_COMMON();

    BOOST_FOREACH(Element *elem, root->get_elements()) {
        if (!readColorInfo(elem)) {
            break;
        }
    }

    BOOST_FOREACH(ColorContainer::value_type v, color_container) {
        color_map[v.first] = v.second;
    }

    return true;
}

bool
ManageIllusionRes::readColorInfo(Element *elem) {
    bool succ = false;
    do {
        uint32 id;
        if (!(getAttrValue(elem, "id", id)
              && (color_container[id].id = id) != 0
              && getAttrValue(elem, "item_get", color_container[id].item_get)
			  && getAttrValue(elem, "for_item", color_container[id].for_item)
              && getAttrValue(elem, "need_streng", color_container[id].sthen_min)
              && getAttrValue(elem, "need_money", color_container[id].consumption))) {
            break;
        }

        char id_key[512];
        char num_key[512];
        BOOST_AUTO(&info, color_container[id]);
        for (int i=1; ; ++i) {
            std::sprintf(id_key, "need_item%d", i);
            std::sprintf(num_key, "need_num%d", i);
            uint32 id, num;
            if (!(getAttrValue(elem, id_key, id) && getAttrValue(elem, num_key, num))) {
                break;
            }
            if (id == 0 || num == 0) {
                continue;
            }
            info.ids.push_back(id);
            info.nums.push_back(num);
        }

        succ = true;
    } while (0);
    return succ;
}

#pragma warning(pop)
