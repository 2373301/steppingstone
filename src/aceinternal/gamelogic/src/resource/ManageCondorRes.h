#ifndef __MANAGE_CONDOR_RES_H_
#define __MANAGE_CONDOR_RES_H_

#include "CondorStruct.h"
#include "CondorStructMap.h"
#pragma warning(push, 2)
#include "typedef.h"
#pragma warning(pop)

#include <boost/ptr_container/ptr_map.hpp>

class Document;
class Element;

class ManageCondorRes {
public:
    bool Load(Document *prey, Document *weapon, Document *formula);

    CondorPreyMap const& getCondorPreyMap();
    CondorWeaponMap const& getCondorWeaponMap();
    CondorFormula const& getCondorFormula();

private:
    bool loadPrey(Document *doc);
    bool loadWeapon(Document *doc);
    bool loadFormula(Document *doc);
    bool readPrey(Element *elem);
    bool readWeapon(Element *elem);
    bool readFormula(Element *elem);

    CondorPreyMap prey_map;
    CondorWeaponMap weapon_map;

    typedef boost::ptr_map<uint32, CondorPrey> PreyContainer;
    typedef boost::ptr_map<uint32, CondorWeapon> WeaponContainer;
    PreyContainer prey_container;
    WeaponContainer weapon_container;

	CondorFormula condor_formula;
};

#endif
