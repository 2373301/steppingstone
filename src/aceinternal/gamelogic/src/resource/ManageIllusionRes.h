#ifndef MANAGE_ILLUSION_H
#define MANAGE_ILLUSION_H

#pragma warning(push, 2)
#include "typedef.h"
#pragma warning(pop)

#include "ColorStruct.h"        // ptr_map need constructor of ColorInfo!
#include "ColorStructMap.h"

#include "boost/ptr_container/ptr_map.hpp"

class Document;
class Element;

class ManageIllusionRes
{
public:

    bool Load(Document *xml);

	ColorInfoMap const& getColorInfoMap();

private:
    bool loadColorInfo(Document *xml);
    bool readColorInfo(Element *elem);

    ColorInfoMap color_map;

    typedef boost::ptr_map<uint32, ColorInfo> ColorContainer;
    ColorContainer color_container;
};

#endif
