#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "ManageCondorRes.h"
#include "CondorStruct.h"

#pragma warning(push, 2)
#include "SimpleXML.h"
#pragma warning(pop)

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

CondorPreyMap const&
ManageCondorRes::getCondorPreyMap() {
    return prey_map;
}

CondorWeaponMap const&
ManageCondorRes::getCondorWeaponMap() {
    return weapon_map;
}

CondorFormula const&
ManageCondorRes::getCondorFormula() {
    return condor_formula;
}

bool
ManageCondorRes::Load(Document *prey, Document *weapon, Document *formula) {
    bool succ = loadPrey(prey) && loadWeapon(weapon) && loadFormula(formula);
    if (!succ) return false;

    BOOST_FOREACH(PreyContainer::value_type v, prey_container) {
        prey_map[v.first] = v.second;
    }

    BOOST_FOREACH(WeaponContainer::value_type v, weapon_container) {
        weapon_map[v.first] = v.second;
    }

    
    return true;
}

bool
ManageCondorRes::loadPrey(Document *doc) {
    DIRTY_COMMON();

    BOOST_FOREACH(Element *elem, root->get_elements()) {
        if (!readPrey(elem)) {
            return false;
        }
    }
    return true;
}

bool
ManageCondorRes::loadWeapon(Document *doc) {
    DIRTY_COMMON();

    BOOST_FOREACH(Element *elem, root->get_elements()) {
        if (!readWeapon(elem)) {
            return false;
        }
    }
    return true;
}

bool
ManageCondorRes::loadFormula(Document *doc) {
    DIRTY_COMMON();

    BOOST_FOREACH(Element *elem, root->get_elements()) {
        if (!readFormula(elem)) {
            return false;
        }
    }
    return true;
}

bool
ManageCondorRes::readFormula(Element *elem) {

	bool succ = true;

	succ = getAttrValue(elem, "times", condor_formula.original_rounds) && succ;
	succ = getAttrValue(elem, "num1", condor_formula.times_per_round) && succ;
	succ = getAttrValue(elem, "pay", condor_formula.resource_type) && succ;
	succ = getAttrValue(elem, "num2", condor_formula.resource_num) && succ;
	
	return succ;
}

bool
ManageCondorRes::readPrey(Element *elem) {
    bool succ = false;
    do {
        uint32 id;
        if (!(getAttrValue(elem, "id", id)
              && (prey_container[id].id = id) != 0
              && getAttrValue(elem, "pro", prey_container[id].zids)
              && getAttrValue(elem, "prova", prey_container[id].pas)
              && getAttrValue(elem, "provb", prey_container[id].pbs)
              && getAttrValue(elem, "score", prey_container[id].score))) {
            break;
        }

        succ = true;
    } while (0);
    return succ;
}

bool
ManageCondorRes::readWeapon(Element *elem) {
    bool succ = false;
    do {
        uint32 id;
        if (!(getAttrValue(elem, "id", id)
              && (weapon_container[id].id = id) != 0
              && getAttrValue(elem, "correct", weapon_container[id].correct)
              && getAttrValue(elem, "vip", weapon_container[id].vip)
              && getAttrValue(elem, "zid", weapon_container[id].zid)
              && getAttrValue(elem, "value", weapon_container[id].value))) {
            break;
        }

        char key[512];
        BOOST_AUTO(&weapon, weapon_container[id]);
        for (int i=1; ; ++i) {
            std::sprintf(key, "rate%d", i);
            uint32 rate;
            if (!getAttrValue(elem, key, rate)) {
                break;
            }
            weapon.rates.push_back(rate);
        }
        if (weapon.rates.size() != prey_container.size()) {
            break;
        }

        succ = true;
    } while (0);
    return succ;
}
