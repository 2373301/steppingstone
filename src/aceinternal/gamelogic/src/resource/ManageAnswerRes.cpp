#include "ManageAnswerRes.h"

#pragma warning(push, 2)
#include "typedef.h"
#include "SimpleXML.h"
#include "Logger.h"
#pragma warning(pop)

#include <cstdio>
#include <vector>
#include <algorithm>
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

namespace {

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

AnswerResource const&
ManageAnswerRes::getAnswerResource() {
    return res;
}

enum {
    NOPTION     = 3,
};

bool
ManageAnswerRes::Load(Document *quest, Document *event, Document *config, Document *coins) {
    bool succ = loadQuestions(quest) && loadEvents(event)
             && loadConfig(config) && loadCoins(coins);
    if (!succ) return false;
    return true;
}

enum { kCoinsIdRefresh = 28, kCoinsIdPowerCD = 29 };
enum { kMinute = 60 * 1000 };

bool
ManageAnswerRes::loadCoins(Document *doc) {
    DIRTY_COMMON();
    uint32 num = 0;
    BOOST_FOREACH(Element *ele, root->get_elements()) {
        do {
            uint32 id, valb;
            if (!(getAttrValue(ele, "functionid", id) && getAttrValue(ele, "b_coin", valb))) {
                break;
            }

            if (id == kCoinsIdRefresh) {
                res.refresh_coins = valb;
                ++num;
            } else if (id == kCoinsIdPowerCD) {
                res.power_coins = valb;
                res.power_unit = 5 * kMinute;
                ++num;
            }
        } while (0);
    }
    return num == 2;
}

bool
ManageAnswerRes::loadConfig(Document *doc) {
    DIRTY_COMMON();
    Element *ele = root->get_element("system");
    if (ele == nullptr) return false;
    uint32 mins, multi;
    if (!(getAttrValue(ele, "world_cd", mins)
          && getAttrValue(ele, "world_multiple", multi))) {
        return false;
    }
    res.power_per_cd = mins * kMinute;
    res.power_top_cd = multi * res.power_per_cd;
    return true;
}

bool
ManageAnswerRes::loadQuestions(Document *doc) {
    DIRTY_COMMON();

    BOOST_FOREACH(Element *ele, root->get_elements()) {
        if (!readQuestion(ele)) {
            return false;
        }
    }

    BOOST_FOREACH(QuestionContainer::value_type v, question_container) {
        res.questions[v.first] = v.second;
    }

    std::set<uint32> sids;
    BOOST_FOREACH(SectionContainer::value_type v, section_container) {
        sids.insert(v.first);
    }
    uint32 i = 0;
    BOOST_FOREACH(uint32 id, sids) {
        if (id != ++i) {
            return false;
        }
        res.sections.push_back(&section_container[id]);
    }

    return true;
}

bool
ManageAnswerRes::readQuestion(Element *ele) {
    bool succ = false;
    do {
        AnswerQuestion q;

        if (!(getAttrValue(ele, "id", q.id)
              && getAttrValue(ele, "mapid", q.section))) {
            break;
        }

        for (int i=1; ; ++i) {
            char k_zid[128];
            char k_arga[128];
            char k_argb[128];
            AnswerOption o;
#pragma warning(push)
#pragma warning(disable : 4251 4996)
            std::sprintf(k_zid, "zid%d", i);
            std::sprintf(k_arga, "answer%da", i);
            std::sprintf(k_argb, "answer%db", i);
#pragma warning(pop)
            if (!(getAttrValue(ele, k_zid, o.zids)
                  && getAttrValue(ele, k_arga, o.argas)
                  && getAttrValue(ele, k_argb, o.argbs))) {
                break;
            }
            if (o.zids.size() != o.argas.size() || o.zids.size() != o.argbs.size()) {
                DEF_LOG_ERROR("number of zid/answer1a/answer1b not matched! qid[%u]\n", q.id);
                break;
            }
            q.options.push_back(o);
        }
        if (q.options.size() != NOPTION) {
            DEF_LOG_ERROR("number of options in question[%u] isn't %d", q.id, NOPTION);
            break;
        }

        question_container[q.id] = q;
        section_container[q.section].id = q.section;
        section_container[q.section].questions.push_back(&question_container[q.id]);
        succ = true;
    } while (0);
    return succ;
}

bool
ManageAnswerRes::loadEvents(Document *doc) {
    DIRTY_COMMON();

    BOOST_FOREACH(Element *ele, root->get_elements()) {
        if (!readEvent(ele)) {
            return false;
        }
    }

    uint32 total = 0;
    uint32 gcd = 0;
    BOOST_FOREACH(EventContainer::value_type v, event_container) {
        uint32 weight = v.second->weight;
        total += weight;
        gcd = GCD(gcd, weight);
    }
    if (total > 10000) {
        return false;
    }
    BOOST_FOREACH(EventContainer::value_type v, event_container) {
        BOOST_AUTO(e, v.second);
        uint32 n = e->weight/gcd;
        while (n--) {
            res.events.push_back(e);
        }
    }
    std::random_shuffle(res.events.begin(), res.events.end());
    res.etotal = 10000/gcd;

    return true;
}

bool
ManageAnswerRes::readEvent(Element *ele) {
    bool succ = false;
    do {
        AnswerEvent e;
        if (!(getAttrValue(ele, "id", e.id)
              && getAttrValue(ele, "zid", e.zid)
              && getAttrValue(ele, "valuea", e.arga)
              && getAttrValue(ele, "valueb", e.argb)
              && getAttrValue(ele, "pro", e.weight))) {
            break;
        }

        event_container[e.id] = e;
        succ = true;
    } while (0);
    return succ;
}
