#pragma warning(push)
#pragma warning(disable : 4251 4996)

#include "ManageChakraRes.h"

#include <algorithm>

#include <boost/foreach.hpp>
#include <boost/typeof/typeof.hpp>

#define nullptr     NULL

namespace {

bool
_chakrp_level_lesser(ChakrpLevel const& a, ChakrpLevel const& b) {
    return a.level < b.level;
}

bool
_chakrp_level_equal(ChakrpLevel const& a, ChakrpLevel const&b ) {
    return a.level == b.level;
}

bool
_chakro_infop_lesser(ChakroInfo *a, ChakroInfo *b) {
    return a->id < b->id;
}

bool
_chakrp_infop_lesser(ChakrpInfo *a, ChakrpInfo *b) {
    return a->id < b->id;
}

uint32
GCD(uint32 a, uint32 b) {
    if (b == 0) return a;
    else return GCD(b, a%b);
}

uint32
GCD(std::vector<uint32> const& weights) {
    uint32 gcd = 0;
    BOOST_FOREACH(uint32 val, weights) {
        gcd = GCD(gcd, val);
    }
    return gcd;
}

}

ManageChakraRes::ManageChakraRes()
    : _chakre_loaded(false), _chakro_loaded(false), _chakrp_loaded(false) {
}

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


bool
ManageChakraRes::LoadChakre(Document *doc) {
    DIRTY_COMMON();

    if (_chakre_loaded) {
        DEF_LOG_ERROR("chakra item loaded\n")
        return false;
    }

    Elements const& elems = root->get_elements();
    for (BOOST_AUTO(it, elems.begin()); it != elems.end(); ++it) {
        if (!newChakreInfo(*it)) {
            clearChakre();
            return false;
        }
    }

    for (BOOST_AUTO(it, _chakreMap.begin()); it != _chakreMap.end(); ++it) {
        ChakreInfo *e = it->second;
        ChakreKey key;
        key.point = e->point;
        key.color = e->color;
        if (_chakreSelectMap.count(key)) {
            clearChakre();
            DEF_LOG_ERROR("duplicated chakra item[point(%u) color(%u)]\n",
                key.point, key.color);
            return false;
        }
        _chakreSelectMap[key] = e;
    }

    _chakre_loaded = true;
    return true;
}

bool
ManageChakraRes::LoadChakro(Document *doc) {
    DIRTY_COMMON();

    if (_chakro_loaded) {
        return false;
    }

    Elements const& elems = root->get_elements();
    for (BOOST_AUTO(it, elems.begin()); it != elems.end(); ++it) {
        if (!newChakroInfo(*it)) {
            clearChakro();
            return false;
        }
    }

    for (BOOST_AUTO(it, _chakroMap.begin()); it != _chakroMap.end(); ++it) {
        ChakroInfo *p = it->second;
        _chakroInfoMap[p->id] = p;
        _chakroSortedInfos.push_back(p);
    }

    std::sort(_chakroSortedInfos.begin(), _chakroSortedInfos.end(), _chakro_infop_lesser);
    for (size_t i=0,n=_chakroSortedInfos.size(); i<n; ++i) {
        _chakroSortedInfos[i]->seq = static_cast<uint32>(i);
        ChakroInfo *next = nullptr;
        if (i+1 < n) {
            next = _chakroSortedInfos[i+1];
        }
        _chakroSortedInfos[i]->next = next;
    }

    return true;
}

bool
ManageChakraRes::LoadChakrp(Document *doc) {
    DIRTY_COMMON();

    if (_chakrp_loaded) {
        return false;
    }

    Elements const& elems = root->get_elements();
    for (BOOST_AUTO(it, elems.begin()); it != elems.end(); ++it) {
        if (!newChakrpInfo(*it)) {
            clearChakrp();
            return false;
        }
    }

    for (BOOST_AUTO(it, _chakrpMap.begin()); it != _chakrpMap.end(); ++it) {
        ChakrpInfo *p = it->second;
        std::sort(p->levels.begin(), p->levels.end(), _chakrp_level_lesser);
        for (size_t i=0, n= p->levels.size(); i<n; ++i) {
            if (p->levels[i].level != static_cast<uint32>(i+1)) {
                clearChakrp();
                DEF_LOG_ERROR("erroneous level infos for chakra group[%u]\n", p->id);
                return false;
            }
        }
        _chakrpInfoMap[p->id] = p;
        _chakrpSortedInfos.push_back(p);
    }

    std::sort(_chakrpSortedInfos.begin(), _chakrpSortedInfos.end(), _chakrp_infop_lesser);
    for (size_t i=0,n=_chakrpSortedInfos.size(); i<n; ++i) {
        _chakrpSortedInfos[i]->seq = static_cast<uint32>(i);
    }

    _chakrp_loaded = true;
    return true;
}

bool
ManageChakraRes::CheckSelfConsistency() {
    // TODO consistency checking
    return true;
}

bool
ManageChakraRes::newChakroInfo(Element *elem) {
    bool succ = false;
    do {
        uint32 id;
        if (!(getAttrValue(elem, "id", id)
              && (_chakroMap[id].id = id) != 0
              && getAttrValue(elem, "bechakra", _chakroMap[id].next_per100)
              && getAttrValue(elem, "renew", _chakroMap[id].acc_top)
              && getAttrValue(elem, "consumption", _chakroMap[id].consumption)
              && getAttrValue(elem, "con_yuanbao", _chakroMap[id].act_coins)
              && getAttrValue(elem, "times", _chakroMap[id].act_max))) {
            break;
        }

        BOOST_AUTO(&rand, _chakroMap[id].rand);
        char key[256];
        uint32 gcd;

        std::vector<uint32> weights;
        for (uint32 i=0; ; ++i) {
            sprintf(key, "getchakra_%u", i);
            uint32 val = 0;
            if (!getAttrValue(elem, key, val)) {
                break;
            }
            weights.push_back(val);
        }
        gcd = GCD(weights);
        for (size_t i=0, n=weights.size(); i<n; ++i) {
            uint32 j = weights[i]/gcd;
            while (j--) {
                rand.colors.push_back(static_cast<uint32>(i));
            }
        }
        std::random_shuffle(rand.colors.begin(), rand.colors.end());

        weights.clear();
        for (uint32 i=1; ; ++i) {
            sprintf(key, "chakraself_%u", i);
            uint32 val = 0;
            if (!getAttrValue(elem, key, val)) {
                break;
            }
            weights.push_back(val);
        }
        gcd = GCD(weights);
        for (size_t i=0, n=weights.size(); i<n; ++i) {
            uint32 j = weights[i]/gcd;
            while (j--) {
                rand.points.push_back(static_cast<uint32>(i+1));
            }
        }
        std::random_shuffle(rand.points.begin(), rand.points.end());

        succ = true;
    } while (0);
    return succ;
}

bool
ManageChakraRes::newChakrpInfo(Element *elem) {
    bool succ = false;
    do {
        struct {
            uint32 id;
            uint32 level;
            uint32 exp;
            uint32 property;
            uint32 propvalue;
            uint32 first_strike;
        } v;

        if (!(getAttrValue(elem, "group", v.id)
              && getAttrValue(elem, "level", v.level)
              && getAttrValue(elem, "exp", v.exp)
              && getAttrValue(elem, "pub", v.property)
              && getAttrValue(elem, "first_strike", v.first_strike)
              && getAttrValue(elem, "num", v.propvalue))) {
            break;
        }

        if (_chakrpMap.count(v.id) == 0) {
            _chakrpMap[v.id].id = v.id;
            _chakrpMap[v.id].property = v.property;
        } else if (v.property != _chakrpMap[v.id].property) {
            DEF_LOG_ERROR("chakrp[%u] property[%u] diff level[%u property(%u)]",
                v.id, _chakrpMap[v.id].property, v.level, v.property);
            return false;
        }

        ChakrpLevel l;
        l.level = v.level;
        l.exp = v.exp;
        l.value = v.propvalue;
        l.first_strike = v.first_strike;
        _chakrpMap[v.id].levels.push_back(l);

        succ = true;
    } while (0);
    return succ;
}

bool
ManageChakraRes::newChakreInfo(Element *elem) {
    bool succ = false;

    do {
        uint32 id;
        if (!(getAttrValue(elem, "id", id)
              && (_chakreMap[id].id = id) != 0
              && getAttrValue(elem, "color", _chakreMap[id].color)
              && getAttrValue(elem, "belong", _chakreMap[id].point)
              && getAttrValue(elem, "exp", _chakreMap[id].exp)
              && getAttrValue(elem, "money", _chakreMap[id].money))) {
            break;
        }

        succ = true;
    } while (0);

    return succ;
}

void
ManageChakraRes::clearChakre() {
    _chakreMap.clear();
    _chakreInfoMap.clear();
}

void
ManageChakraRes::clearChakro() {
    _chakroMap.clear();
    _chakroInfoMap.clear();
}

void
ManageChakraRes::clearChakrp() {
    _chakrpMap.clear();
    _chakrpInfoMap.clear();
}

ChakreInfoMap const&
ManageChakraRes::getChakreInfoMap() {
    return _chakreInfoMap;
}

ChakreSelectMap const&
ManageChakraRes::getChakreSelectMap() {
    return _chakreSelectMap;
}

ChakroInfoMap const&
ManageChakraRes::getChakroInfoMap() {
    return _chakroInfoMap;
}

ChakroSortedVec const&
ManageChakraRes::getChakroSortedInfos() {
    return _chakroSortedInfos;
}

ChakrpInfoMap const&
ManageChakraRes::getChakrpInfoMap() {
    return _chakrpInfoMap;
}

ChakrpSortedVec const&
ManageChakraRes::getChakrpSortedInfos() {
    return _chakrpSortedInfos;
}

#pragma warning(pop)
