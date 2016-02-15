#ifndef __MANAGE_CHAKRA_RES_HPP
#define __MANAGE_CHAKRA_RES_HPP

#pragma warning(push, 2)
#include "typedef.h"
#include "SimpleXML.h"
#include "entity_ins.h"
#include "Resource.h"
#include "gamelogic_def.h"
#include "Logger.h"
#pragma warning(pop)

#include <boost/unordered_map.hpp>
#include <boost/ptr_container/ptr_map.hpp>

class GAMELOGIC_EXOPRT ManageChakraRes {
public:
    ManageChakraRes();

    bool LoadChakre(Document *doc);
    bool LoadChakrp(Document *doc);
    bool LoadChakro(Document *doc);

    // After module's all resources loaded.
    bool CheckSelfConsistency();

    ChakreInfoMap const& getChakreInfoMap();
    ChakreSelectMap const& getChakreSelectMap();

    ChakroInfoMap const& getChakroInfoMap();
    ChakroSortedVec const& getChakroSortedInfos();

    ChakrpInfoMap const& getChakrpInfoMap();
    ChakrpSortedVec const& getChakrpSortedInfos();

private:
    boost::ptr_map<uint32, ChakreInfo> _chakreMap;
    boost::ptr_map<uint32, ChakroInfo> _chakroMap;
    boost::ptr_map<uint32, ChakrpInfo> _chakrpMap;

    ChakreInfoMap _chakreInfoMap;
    ChakreSelectMap _chakreSelectMap;

    ChakroInfoMap _chakroInfoMap;
    ChakroSortedVec _chakroSortedInfos;

    ChakrpInfoMap _chakrpInfoMap;
    ChakrpSortedVec _chakrpSortedInfos;

    void clearChakre();
    void clearChakro();
    void clearChakrp();
    bool newChakreInfo(Element *elem);
    bool newChakroInfo(Element *elem);
    bool newChakrpInfo(Element *elem);

    bool _chakre_loaded;
    bool _chakro_loaded;
    bool _chakrp_loaded;
};

#endif
